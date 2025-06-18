// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "CoopPuzzle/Subsystem/LevelSequenceSubsystem.h"

DEFINE_LOG_CATEGORY( LogEventTriggerManagerSubsystem );

void UEventTriggerManagerSubsystem::TriggerManualEvent( int64 iPlayerUID, EManualTriggerMode eEventTriggerMode )
{
    const int64* pEventTriggerUID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID );
    if( pEventTriggerUID == nullptr )
        return;

    TriggerEvent( iPlayerUID, *pEventTriggerUID, true, eEventTriggerMode );
}

void UEventTriggerManagerSubsystem::TriggerAutoEvent( int64 iPlayerUID, EEventTriggerCondition eConditionType, const FName& ConditionKey )
{
    TMap<FName, FEventTriggerConditionParams>* pTriggerConditions = mapCachedAutoTriggerConditions.Find( eConditionType );
    if( pTriggerConditions == nullptr || pTriggerConditions->Num() <= 0 )
        return;

    TArray<FName> arrSuccessTriggerIDs;
    for( const auto& TriggerCondition : *pTriggerConditions )
    {
        const FName& TriggerID = TriggerCondition.Key;
        const TMap<FName, int32>& mapConditions = TriggerCondition.Value.Params;

        if( mapConditions.Contains( ConditionKey ) == false )
            continue;

        // 해당 ID의 트리거가 하나도 없다면 continue
        const TSet<int64>* setTriggerUIDs = mapEventTriggerUIDs.Find( TriggerID );
        if( setTriggerUIDs == nullptr || setTriggerUIDs->IsEmpty() == true )
            continue;

        bool bForceComplete = false;
        for( int64 iTriggerUID : *setTriggerUIDs )
        {
            if( bForceComplete == false )
            {
                if( TriggerEvent( iPlayerUID, iTriggerUID, false ) != EEventTriggerResult::Success )
                    break;

                bForceComplete = true;
                arrSuccessTriggerIDs.Add( TriggerCondition.Key );
                continue;
            }

            // 동일 ID 트리거 강제 Success 처리
            mapCachedTriggerHandle.FindRef( iTriggerUID ).OnCompletedDelegate.ExecuteIfBound( EEventTriggerResult::Success );
        }
    }

    for( const FName& SuccessTriggerID : arrSuccessTriggerIDs )
    {
        pTriggerConditions->Remove( SuccessTriggerID );
    }
}

void UEventTriggerManagerSubsystem::RegisterEventTrigger( AEventTriggerObjectBase* pEventTrigger, const FOnEventTriggerCompleted& OnCompletedCallback )
{
    checkf( IsValid( pEventTrigger ) == true, TEXT( "pEventTrigger is not valid." ) );

    mapCachedTriggerHandle.Add( pEventTrigger->GetEventTriggerUID_DE(), FEventTriggerHandle( pEventTrigger, OnCompletedCallback ) );

    mapEventTriggerIDs.Add( pEventTrigger->GetEventTriggerUID_DE(), pEventTrigger->GetEventTriggerID() );
    mapEventTriggerUIDs.FindOrAdd( pEventTrigger->GetEventTriggerID() ).Add( pEventTrigger->GetEventTriggerUID_DE() );

    // 조건 충족 시 즉시 실행해야 하는 트리거 정보 캐싱
    if( pEventTrigger->GetTriggerData()->AutoTriggerOnCondition == true )
    {
        const FEventTriggerConditionParams& TriggerCondition = pEventTrigger->GetTriggerData()->EventTriggerCondition;
        mapCachedAutoTriggerConditions.FindOrAdd( TriggerCondition.ConditionType ).Add( pEventTrigger->GetEventTriggerID(), TriggerCondition );
    }
}

void UEventTriggerManagerSubsystem::UnregisterEventTrigger( int64 iEventTriggerUID )
{
    mapEventTriggerUIDs.FindOrAdd( mapEventTriggerIDs.FindRef( iEventTriggerUID ) ).Remove( iEventTriggerUID );
	mapEventTriggerIDs.Remove( iEventTriggerUID );

    // 트리거 연결 정보 정리
    for( int64 iPlayerUID : mapLinkedEventTriggerToPlayer.FindRef( iEventTriggerUID ) )
    {
        mapLinkedPlayerToEventTrigger.Remove( iPlayerUID );
    }
    mapLinkedEventTriggerToPlayer.Remove( iEventTriggerUID );
}

void UEventTriggerManagerSubsystem::LinkPlayerToEventTrigger( int64 iPlayerUID, int64 EventTriggerUID )
{
    // 이미 링크 등록되었다면 동작하지 않음
    if( const int64* pOldEventTriggerUID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID ) )
    {
        if( *pOldEventTriggerUID == EventTriggerUID )
            return;

        UnlinkPlayerToEventTrigger( iPlayerUID, *pOldEventTriggerUID );
    }

    // 플레이어 <-> 트리거 연결 정보 갱신
    mapLinkedPlayerToEventTrigger.Add( iPlayerUID, EventTriggerUID );
    mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerUID ).Add( iPlayerUID );

    // 연결과 함께 이벤트 실행해야 하는 모드일 경우
    const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( EventTriggerUID );
    if( pEventTriggerHandle != nullptr && pEventTriggerHandle->EventTrigger.IsValid() == true )
    {
        const FEventTriggerDataRow* pEventTriggerData = pEventTriggerHandle->EventTrigger.Get()->GetTriggerData();
        switch( pEventTriggerData->ManualTriggerMode )
        {
        case EManualTriggerMode::InputInteractKey:
        {
            if( pEventTriggerHandle->EventTrigger.Get()->GetTriggerState() != EEventTriggerState::Enabled )
                break;

            UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
            if( IsValid( pWidgetDelegateSubsystem ) == false )
                break;

            pWidgetDelegateSubsystem->OnShowPlayerInputIcon_ToClient.FindOrAdd( iPlayerUID ).Broadcast( EPlayerInputType::Interact, true );
        } break;
        case EManualTriggerMode::InTriggerVolume_Once:
        case EManualTriggerMode::InTriggerVolume_Stay:
        case EManualTriggerMode::InTriggerVolume_Toggle:
        {
            TriggerManualEvent( iPlayerUID, pEventTriggerData->ManualTriggerMode );
        } break;
        }
    }
}

void UEventTriggerManagerSubsystem::UnlinkPlayerToEventTrigger( int64 iPlayerUID, int64 EventTriggerUID )
{
    // 이미 링크 해제되었다면 동작하지 않음
    if( mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerUID ).Contains( iPlayerUID ) == false )
        return;

    // 플레이어 <-> 트리거 연결 정보 갱신
	const int64* pEventTriggerUID = mapLinkedPlayerToEventTrigger.Find( iPlayerUID );
    if( pEventTriggerUID == nullptr || *pEventTriggerUID != EventTriggerUID )
        return;

	mapLinkedPlayerToEventTrigger.Remove( iPlayerUID );
	mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerUID ).Remove( iPlayerUID );

    // 해제와 함께 이벤트 실행해야 하는 모드일 경우
    const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( EventTriggerUID );
    if( pEventTriggerHandle != nullptr && pEventTriggerHandle->EventTrigger.IsValid() == true )
    {
        const FEventTriggerDataRow* pEventTriggerData = pEventTriggerHandle->EventTrigger.Get()->GetTriggerData();
        switch( pEventTriggerData->ManualTriggerMode )
        {
        case EManualTriggerMode::InputInteractKey:
        {
            UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
            if( IsValid( pWidgetDelegateSubsystem ) == false )
                break;

            pWidgetDelegateSubsystem->OnShowPlayerInputIcon_ToClient.FindOrAdd( iPlayerUID ).Broadcast( EPlayerInputType::Interact, false );
        } break;
        case EManualTriggerMode::InTriggerVolume_Stay:
        {
            if( mapLinkedEventTriggerToPlayer.FindOrAdd( EventTriggerUID ).Num() > 0 )
                break;
            
            //트리거에 연결된 유저가 아무도 없을 때 초기화
            pEventTriggerHandle->OnCompletedDelegate.ExecuteIfBound( EEventTriggerResult::Failed );
		} break;
        }
    }
}

bool UEventTriggerManagerSubsystem::ShouldCreateSubsystem( UObject* Outer ) const
{
    const UGameInstance* pGameInstance = Cast<UGameInstance>( Outer );
    return IsValid( pGameInstance ) == true && pGameInstance->IsDedicatedServerInstance() == true;
}

EEventTriggerResult UEventTriggerManagerSubsystem::TriggerEvent( int64 iPlayerUID, int64 EventTriggerUID, bool bIsManualTriggerd, EManualTriggerMode eEventTriggerMode )
{
    UE_LOG( LogTemp, Error, TEXT( "EventTriggerUID: %d" ), EventTriggerUID );

    const FEventTriggerHandle* pEventTriggerHandle = mapCachedTriggerHandle.Find( EventTriggerUID );
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
            const TMap<FName, int32>& mapTriggerCount = pEventTriggerData->EventTriggerCondition.Params;
            if( mapTriggerCount.IsEmpty() == true )
                break;

            int32 iRemainingTriggerCount = mapTriggerCount.Num();
            for( const TPair<FName, int32>& TriggerID : mapTriggerCount )
            {
                checkf( TriggerID.Value > 0, TEXT( "Trigger count must be greater than zero." ) );

                // 등록된 해당 ID의 트리거가 필요한 트리거 개수보다 적다면 실패
                const TSet<int64>* setTriggerUIDs = mapEventTriggerUIDs.Find( TriggerID.Key );
                if( setTriggerUIDs == nullptr || setTriggerUIDs->Num() < TriggerID.Value )
                    break;

                int iRemainingCount = TriggerID.Value;
                for( int64 iTriggerUID : *setTriggerUIDs )
                {
                    const FEventTriggerHandle* pTriggerHandle = mapCachedTriggerHandle.Find( iTriggerUID );
                    if( pTriggerHandle == nullptr )
                        continue;

                    AEventTriggerObjectBase* pOtherEventTrigger = pTriggerHandle->EventTrigger.Get();
                    if( IsValid( pOtherEventTrigger ) == false || pOtherEventTrigger->GetTriggerState() != EEventTriggerState::Triggered )
                        continue;

                    // 해당 ID의 트리거 개수를 충족했다면 for문 나가기
                    if( --iRemainingCount <= 0 )
                        break;
                }

                // 해당 ID의 트리거 개수를 충족하지 못했다면 실패
                if( iRemainingCount > 0 )
                    break;

				--iRemainingTriggerCount;
            }

            // 트리거 횟수를 충족하지 못했다면 실패
            if( iRemainingTriggerCount > 0 )
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
        {
            pWidgetDelegateSubsystem->OnShowGlobalNotification_ToClient.Broadcast( *pGlobalNotification );
        }

        if( const FText* pLocalNotification = pEventTriggerData->LocalNotifications.Find( eResult ) )
        {
            pWidgetDelegateSubsystem->OnShowLocalNotification_ToClient.FindOrAdd( iPlayerUID ).Broadcast( *pLocalNotification );
        }

        // 트리거 성공 시 연결된 유저들에게 키 알림 off 요청
        if( eResult == EEventTriggerResult::Success && pEventTriggerData->ManualTriggerMode == EManualTriggerMode::InputInteractKey )
        {
            for( int64 iLinkedPlayerUID : mapLinkedEventTriggerToPlayer.FindRef( EventTriggerUID ) )
            {
                pWidgetDelegateSubsystem->OnShowPlayerInputIcon_ToClient.FindOrAdd( iLinkedPlayerUID ).Broadcast( EPlayerInputType::Interact, false );
            }
        }
    }

    pEventTriggerHandle->OnCompletedDelegate.ExecuteIfBound( eResult );

    return eResult;
}