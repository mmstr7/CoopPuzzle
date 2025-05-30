// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Object/EventTriggerObjectBase.h"

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

    pCompletedDelegate->ExecuteIfBound( EEventTriggerResult::Success );
}

void UEventTriggerManagerSubsystem::RegisterEventTrigger( const FName& EventTriggerID, FOnEventTriggerCompleted OnCompletedCallback )
{
    mapTriggerCompletedDelegates.Add( EventTriggerID, OnCompletedCallback );
}

void UEventTriggerManagerSubsystem::UnregisterEventTrigger( const FName& EventTriggerID )
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
