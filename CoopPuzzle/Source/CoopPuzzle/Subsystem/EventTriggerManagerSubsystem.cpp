// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Subsystem/WorldActorManagerSubsystem.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"

DEFINE_LOG_CATEGORY( LogEventTriggerManagerSubsystem );

void UEventTriggerManagerSubsystem::TriggerEvent( const int64& iPlayerUID, EEventTriggerMode eEventTriggerMode )
{
    const FName* pEventTriggerID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID );
    if( pEventTriggerID == nullptr )
        return;

    const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( *pEventTriggerID );
    if( pEventTriggerHandle == nullptr )
        return;

    AEventTriggerObjectBase* pEventTrigger = pEventTriggerHandle->EventTrigger.Get();
    if( IsValid( pEventTrigger ) == false )
        return;

    // 스테이트 체크
    if( pEventTrigger->GetTriggerState() != EEventTriggerState::Enabled )
        return;

    // 실행 모드가 일치하는지 확인
    const FEventTriggerDataRow* pEventTriggerData = pEventTrigger->GetTriggerData();
    if( pEventTriggerData->EventTriggerMode != eEventTriggerMode )
        return;

    // 조건 체크
    EEventTriggerResult eResult = EEventTriggerResult::Failed;
    switch( pEventTriggerData->EventTriggerCondition.ConditionType )
    {
    case EEventTriggerCondition::Unconditional:
    {
        eResult = EEventTriggerResult::Success;
    } break;
    case EEventTriggerCondition::OverlapConditionVolume:
    {
        UWorldActorManagerSubsystem* pWorldActorManager = GetGameInstance()->GetSubsystem<UWorldActorManagerSubsystem>();
        if( IsValid( pWorldActorManager ) == false )
            break;

        TWeakObjectPtr<ACoopPuzzleCharacter> pPlayer = pWorldActorManager->GetPlayer( iPlayerUID );
        if( pPlayer.IsValid() == false )
            break;

        if( pEventTrigger->IsConditionOverlappingPlayer( pPlayer.Get() ) == false )
            break;

        eResult = EEventTriggerResult::Success;
    } break;
    case EEventTriggerCondition::HasItem_Consume:
    {
        UItemSubsystem* pItemSubsystem = GetGameInstance()->GetSubsystem<UItemSubsystem>();
        if( IsValid( pItemSubsystem ) == false )
            break;

        TMap<FName, int32> mapConsumeItems = pEventTriggerData->EventTriggerCondition.Params;
        for( TPair<FName, int32>& pairConsumeItem : mapConsumeItems )
        {
            checkf( pairConsumeItem.Value > 0, TEXT( "Item count must be greater than zero. (ItemID: %s)" ), *pairConsumeItem.Key.ToString() );
            pairConsumeItem.Value *= -1;
        }

        // 필요한 아이템을 모두 사용 할 수 없다면 실패
        if( pItemSubsystem->AddItems_DE( iPlayerUID, mapConsumeItems ) == false )
            break;

        eResult = EEventTriggerResult::Success;
    } break;
    case EEventTriggerCondition::HasItem_Possession:
    {
        UItemSubsystem* pItemSubsystem = GetGameInstance()->GetSubsystem<UItemSubsystem>();
        if( IsValid( pItemSubsystem ) == false )
            break;

        // 필요한 아이템을 모두 소지하고 있지 않다면 실패
        if( pItemSubsystem->HasItems( iPlayerUID, pEventTriggerData->EventTriggerCondition.Params ) == false )
            break;
        
        eResult = EEventTriggerResult::Success;
    } break;
    default:
    {
        // checkf 출력 필요
    } break;
    }

    // 성공 시 효과 실행
    if( eResult == EEventTriggerResult::Success )
    {
        switch( pEventTriggerData->EventTriggerSuccessEffect.EffectType )
        {
        case EEventTriggerSuccessEffect::None:
        {

        } break;
        case EEventTriggerSuccessEffect::GainItem:
        {
            UItemSubsystem* pItemSubsystem = GetGameInstance()->GetSubsystem<UItemSubsystem>();
            if( IsValid( pItemSubsystem ) == false )
                break;

            for( const TPair<FName, int32>& pairItemToAdd : pEventTriggerData->EventTriggerSuccessEffect.Params )
            {
                checkf( pairItemToAdd.Value > 0, TEXT( "Item count must be greater than zero. (ItemID: %s)" ), *pairItemToAdd.Key.ToString() );
            }

            bool bAddSuccess = pItemSubsystem->AddItems_DE( iPlayerUID, pEventTriggerData->EventTriggerSuccessEffect.Params );
            checkf( bAddSuccess == true, TEXT( "GainItem Failed!" ) );
        } break;
        default:
        {
            // checkf 출력 필요
        } break;
        }
    }

    // 위젯 알림 출력
    UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>();
    if( IsValid( pWidgetDelegateSubsystem ) == true )
    {
        if( const FText* pGlobalNotification = pEventTriggerData->GlobalNotifications.Find( eResult ) )
            pWidgetDelegateSubsystem->OnShowGlobalNotification.Broadcast( *pGlobalNotification );

        if( const FText* pLocalNotification = pEventTriggerData->LocalNotifications.Find( eResult ) )
            pWidgetDelegateSubsystem->OnShowLocalNotification.FindOrAdd( iPlayerUID ).Broadcast( *pLocalNotification );
    }

    pEventTriggerHandle->OnCompletedDelegate.ExecuteIfBound( eResult );
}

void UEventTriggerManagerSubsystem::RegisterEventTriggerHandle( AEventTriggerObjectBase* pEventTrigger, FOnEventTriggerCompleted OnCompletedCallback )
{
    checkf( IsValid( pEventTrigger ) == true, TEXT( "pEventTrigger is not valid." ) );

    mapCachedTriggerHandle.Add( pEventTrigger->GetEventTriggerID(), FEventTriggerHandle( pEventTrigger, OnCompletedCallback ) );
}

void UEventTriggerManagerSubsystem::UnregisterEventTriggerHandle( const FName& EventTriggerID )
{
    mapCachedTriggerHandle.Remove( EventTriggerID );

    // 트리거 연결 정보 정리
    for( int64 iPlayerUID : mapLinkedEventTriggerToPlayer.FindRef( EventTriggerID ) )
    {
        mapLinkedPlayerToEventTrigger.Remove( iPlayerUID );
    }
    mapLinkedEventTriggerToPlayer.Remove( EventTriggerID );
}

void UEventTriggerManagerSubsystem::LinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID )
{
    // 플레이어 <-> 트리거 연결 정보 갱신
    if( const FName* pOldEventTriggerID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID ) )
    {
        mapLinkedEventTriggerToPlayer.FindOrAdd( *pOldEventTriggerID ).Remove( iPlayerUID );
    }

    mapLinkedPlayerToEventTrigger.Add( iPlayerUID, EventTriggerID );
    mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerID ).Add( iPlayerUID );

    // 연결과 함께 이벤트 실행해야 하는 모드일 경우
    const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( EventTriggerID );
    if( pEventTriggerHandle != nullptr && pEventTriggerHandle->EventTrigger.IsValid() == true )
    {
        const FEventTriggerDataRow* pEventTriggerData = pEventTriggerHandle->EventTrigger.Get()->GetTriggerData();
        if( pEventTriggerData->EventTriggerMode == EEventTriggerMode::InTriggerVolume_Once ||
            pEventTriggerData->EventTriggerMode == EEventTriggerMode::InTriggerVolume_Stay )
        {
            TriggerEvent( iPlayerUID, pEventTriggerData->EventTriggerMode );
        }
    }
}

void UEventTriggerManagerSubsystem::UnlinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID )
{
    // 플레이어 <-> 트리거 연결 정보 갱신
    mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerID ).Remove( iPlayerUID );

    const FName* pEventTriggerID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID );
    if( pEventTriggerID != nullptr && *pEventTriggerID == EventTriggerID )
    {
        mapLinkedPlayerToEventTrigger.Remove( iPlayerUID );
    }

    // 트리거에 연결된 유저가 아무도 없을 때 초기화 해야 하는 모드일 경우
    if( mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerID ).Num() <= 0 )
    {
        const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( EventTriggerID );
        if( pEventTriggerHandle == nullptr || pEventTriggerHandle->EventTrigger.IsValid() == false )
            return;

        const FEventTriggerDataRow* pEventTriggerData = pEventTriggerHandle->EventTrigger.Get()->GetTriggerData();
        if( pEventTriggerData->EventTriggerMode == EEventTriggerMode::InTriggerVolume_Stay )
        {
            pEventTriggerHandle->OnCompletedDelegate.ExecuteIfBound( EEventTriggerResult::Reset );
        }
    }
}

bool UEventTriggerManagerSubsystem::ShouldCreateSubsystem( UObject* Outer ) const
{
    const UGameInstance* pGameInstance = Cast<UGameInstance>( Outer );
    return IsValid( pGameInstance ) == true && pGameInstance->IsDedicatedServerInstance() == true;
}
