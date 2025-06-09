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

    UDataTableSubsystem* pDataTableSubsystem = GetGameInstance()->GetSubsystem<UDataTableSubsystem>();
    checkf( IsValid( pDataTableSubsystem ) == true, TEXT( "DataTableSubsystem is not valid. Please check class." ) );

    const FEventTriggerDataRow* pEventTriggerData = pDataTableSubsystem->GetDataRowOrNull<FEventTriggerDataRow>( EDataTableType::EventTrigger, *pEventTriggerID );
    checkf( pEventTriggerData != nullptr, TEXT( "EventTriggerID [%s] is not valid. Please check EventDataTable." ), *pEventTriggerID->ToString() );

    const FOnEventTriggerCompleted* pCompletedDelegate = mapTriggerCompletedDelegates.Find( *pEventTriggerID );
    if( pCompletedDelegate == nullptr )
        return;

    // 실행 모드가 일치하는지 확인
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
        TWeakObjectPtr<AEventTriggerObjectBase> pEventTrigger = pWorldActorManager->GetEventTrigger( *pEventTriggerID );
        if( pPlayer.IsValid() == false || pEventTrigger.IsValid() == false )
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

    pCompletedDelegate->ExecuteIfBound( eResult );
}

void UEventTriggerManagerSubsystem::RegisterEventTriggerCallback( const FName& EventTriggerID, FOnEventTriggerCompleted OnCompletedCallback )
{
    mapTriggerCompletedDelegates.Add( EventTriggerID, OnCompletedCallback );
}

void UEventTriggerManagerSubsystem::UnregisterEventTriggerCallback( const FName& EventTriggerID )
{
    mapTriggerCompletedDelegates.Remove( EventTriggerID );
}

void UEventTriggerManagerSubsystem::LinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID )
{
    mapLinkedPlayerToEventTrigger.Add( iPlayerUID, EventTriggerID );
}

void UEventTriggerManagerSubsystem::UnlinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID )
{
    const FName* pEventTriggerID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID );
    if( pEventTriggerID == nullptr || *pEventTriggerID != EventTriggerID )
        return;

    mapLinkedPlayerToEventTrigger.Remove( iPlayerUID );
}

bool UEventTriggerManagerSubsystem::ShouldCreateSubsystem( UObject* Outer ) const
{
    const UGameInstance* pGameInstance = Cast<UGameInstance>( Outer );
    return IsValid( pGameInstance ) == true && pGameInstance->IsDedicatedServerInstance() == true;
}
