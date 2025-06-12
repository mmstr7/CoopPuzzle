// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Subsystem/WorldActorManagerSubsystem.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "CoopPuzzle/Subsystem/LevelSequenceSubsystem.h"

DEFINE_LOG_CATEGORY( LogEventTriggerManagerSubsystem );

void UEventTriggerManagerSubsystem::TriggerManualEvent( const int64& iPlayerUID, EManualTriggerMode eEventTriggerMode )
{
    const FName* pEventTriggerID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID );
    if( pEventTriggerID == nullptr )
        return;

    TriggerEvent( iPlayerUID, *pEventTriggerID, true, eEventTriggerMode );
}

void UEventTriggerManagerSubsystem::TriggerAutoEvent( const int64& iPlayerUID, EEventTriggerCondition eConditionType, const FName& ConditionKey )
{
    TMap<FName, FEventTriggerConditionParams>* pTriggerConditions = mapCachedAutoTriggerConditions.Find( eConditionType );
    if( pTriggerConditions == nullptr || pTriggerConditions->Num() <= 0 )
        return;

    TArray<FName> arrSuccessTriggerIDs;
    for( const auto& TriggerCondition : *pTriggerConditions )
    {
        if( TriggerCondition.Value.Params.Contains( ConditionKey ) == false )
            continue;

        if( TriggerEvent( iPlayerUID, TriggerCondition.Key, false ) != EEventTriggerResult::Success )
            continue;

        arrSuccessTriggerIDs.Add( TriggerCondition.Key );
    }

    for( const FName& SuccessTriggerID : arrSuccessTriggerIDs )
    {
        pTriggerConditions->Remove( SuccessTriggerID );
    }
}

void UEventTriggerManagerSubsystem::RegisterEventTrigger( AEventTriggerObjectBase* pEventTrigger, const FOnEventTriggerCompleted& OnCompletedCallback )
{
    checkf( IsValid( pEventTrigger ) == true, TEXT( "pEventTrigger is not valid." ) );

    mapCachedTriggerHandle.Add( pEventTrigger->GetEventTriggerID(), FEventTriggerHandle( pEventTrigger, OnCompletedCallback ) );

    // 조건 충족 시 즉시 실행해야 하는 트리거 정보 캐싱
    if( pEventTrigger->GetTriggerData()->AutoTriggerOnCondition == true )
    {
        const FEventTriggerConditionParams& TriggerCondition = pEventTrigger->GetTriggerData()->EventTriggerCondition;
        mapCachedAutoTriggerConditions.FindOrAdd( TriggerCondition.ConditionType).Add( pEventTrigger->GetEventTriggerID(), TriggerCondition );
    }
}

void UEventTriggerManagerSubsystem::UnregisterEventTrigger( const FName& EventTriggerID )
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
    // 이미 링크 등록되었다면 동작하지 않음
    if( const FName* pOldEventTriggerID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID ) )
    {
        if( *pOldEventTriggerID == EventTriggerID )
            return;

        UnlinkPlayerToEventTrigger( iPlayerUID, *pOldEventTriggerID );
    }

    // 플레이어 <-> 트리거 연결 정보 갱신
    mapLinkedPlayerToEventTrigger.Add( iPlayerUID, EventTriggerID );
    mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerID ).Add( iPlayerUID );

    // 연결과 함께 이벤트 실행해야 하는 모드일 경우
    const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( EventTriggerID );
    if( pEventTriggerHandle != nullptr && pEventTriggerHandle->EventTrigger.IsValid() == true )
    {
        const FEventTriggerDataRow* pEventTriggerData = pEventTriggerHandle->EventTrigger.Get()->GetTriggerData();
        switch( pEventTriggerData->ManualTriggerMode )
        {
        case EManualTriggerMode::InTriggerVolume_Once:
        case EManualTriggerMode::InTriggerVolume_Stay:
        case EManualTriggerMode::InTriggerVolume_Toggle:
        {
            TriggerManualEvent( iPlayerUID, pEventTriggerData->ManualTriggerMode );
        } break;
        }
    }
}

void UEventTriggerManagerSubsystem::UnlinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID )
{
    // 이미 링크 해제되었다면 동작하지 않음
    if( mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerID ).Contains( iPlayerUID ) == false )
        return;

    // 플레이어 <-> 트리거 연결 정보 갱신
	const FName* pEventTriggerID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID );
    if( pEventTriggerID == nullptr || *pEventTriggerID != EventTriggerID )
        return;

	mapLinkedPlayerToEventTrigger.Remove( iPlayerUID );
	mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerID ).Remove( iPlayerUID );

	//트리거에 연결된 유저가 아무도 없을 때 초기화 해야 하는 모드일 경우
	if( mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerID ).Num() <= 0 )
	{
		const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( EventTriggerID );
		if( pEventTriggerHandle == nullptr || pEventTriggerHandle->EventTrigger.IsValid() == false )
			return;

		const FEventTriggerDataRow* pEventTriggerData = pEventTriggerHandle->EventTrigger.Get()->GetTriggerData();
		if( pEventTriggerData->ManualTriggerMode == EManualTriggerMode::InTriggerVolume_Stay )
		{
			pEventTriggerHandle->OnCompletedDelegate.ExecuteIfBound( EEventTriggerResult::Failed );
		}
	}
}

bool UEventTriggerManagerSubsystem::ShouldCreateSubsystem( UObject* Outer ) const
{
    const UGameInstance* pGameInstance = Cast<UGameInstance>( Outer );
    return IsValid( pGameInstance ) == true && pGameInstance->IsDedicatedServerInstance() == true;
}

EEventTriggerResult UEventTriggerManagerSubsystem::TriggerEvent( const int64& iPlayerUID, const FName& EventTriggerID, bool bIsManualTriggerd, EManualTriggerMode eEventTriggerMode )
{
    const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( EventTriggerID );
    if( pEventTriggerHandle == nullptr )
        return EEventTriggerResult::Failed;

    AEventTriggerObjectBase* pEventTrigger = pEventTriggerHandle->EventTrigger.Get();
    if( IsValid( pEventTrigger ) == false )
        return EEventTriggerResult::Failed;

    // 스테이트 체크
    if( eEventTriggerMode != EManualTriggerMode::InTriggerVolume_Toggle )
    {
        if( pEventTrigger->GetTriggerState() != EEventTriggerState::Enabled )
            return EEventTriggerResult::Failed;
    }

    const FEventTriggerDataRow* pEventTriggerData = pEventTrigger->GetTriggerData();

    // 실행 모드가 일치하는지 확인
    if( bIsManualTriggerd == true && pEventTriggerData->ManualTriggerMode != eEventTriggerMode )
        return EEventTriggerResult::Failed;

    // 조건 체크
    EEventTriggerResult eResult = EEventTriggerResult::Success;
    if( pEventTriggerData->Unconditional == false )
    {
        eResult = EEventTriggerResult::Failed;

        switch( pEventTriggerData->EventTriggerCondition.ConditionType )
        {
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
        case EEventTriggerCondition::OtherTrigger_Triggered:
        {
            TSet<FName> setTriggerIDs;
            pEventTriggerData->EventTriggerCondition.Params.GetKeys( setTriggerIDs );
            if( setTriggerIDs.Num() <= 0 )
                break;

            int32 iTriggeredCount = 0;
            for( const FName& TriggerID : setTriggerIDs )
            {
                const FEventTriggerHandle* pTriggerHandle = mapCachedTriggerHandle.Find( TriggerID );
                if( pTriggerHandle == nullptr )
                    break;

                AEventTriggerObjectBase* pOtherEventTrigger = pTriggerHandle->EventTrigger.Get();
                if( IsValid( pOtherEventTrigger ) == false || pOtherEventTrigger->GetTriggerState() != EEventTriggerState::Triggered )
                    break;

                ++iTriggeredCount;
            }

            if( iTriggeredCount < setTriggerIDs.Num() )
                break;

            eResult = EEventTriggerResult::Success;
        } break;
        default:
        {
            // checkf 출력 필요
        } break;
        }
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
        case EEventTriggerSuccessEffect::PlayLevelSequence:
        {
            ULevelSequenceSubsystem* pLevelSequenceSubsystem = GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>();
            if( IsValid( pLevelSequenceSubsystem ) == false )
                break;

            // 한 번만 실행
            for( const TPair<FName, int32>& pairLevelSequenceID : pEventTriggerData->EventTriggerSuccessEffect.Params )
            {
                pLevelSequenceSubsystem->PlayLevelSequence_DE( pairLevelSequenceID.Key );
                break;
            }

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
            pWidgetDelegateSubsystem->OnShowGlobalNotification_ToClient.Broadcast( *pGlobalNotification );

        if( const FText* pLocalNotification = pEventTriggerData->LocalNotifications.Find( eResult ) )
            pWidgetDelegateSubsystem->OnShowLocalNotification_ToClient.FindOrAdd( iPlayerUID ).Broadcast( *pLocalNotification );
    }

    pEventTriggerHandle->OnCompletedDelegate.ExecuteIfBound( eResult );

    return eResult;
}
