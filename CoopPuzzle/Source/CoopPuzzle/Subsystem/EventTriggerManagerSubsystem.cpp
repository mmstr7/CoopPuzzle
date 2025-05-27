// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"

DEFINE_LOG_CATEGORY( LogEventTriggerManagerSubsystem );

void UEventTriggerManagerSubsystem::TriggerEvent( const FName& TriggerID, const ACoopPuzzleCharacter* pInstigator ) const
{
   checkf( IsValid( pInstigator ) == true, TEXT( "pInstigator is invalid. Please check the call site." ) );

   UDataTableSubsystem* pDataTableSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UDataTableSubsystem>() : nullptr;
   checkf( IsValid( pDataTableSubsystem ) == true, TEXT( "pDataTableSubsystem is invalid." ) );

   const FEventDataRow* pEventData = pDataTableSubsystem->GetDataRowOrNull<FEventDataRow>( EDataTableType::Event, TriggerID );
   checkf( pEventData != nullptr, TEXT( "pEventData is invalid. Please check TriggerID" ) );

   // TODO: 트리거 조건 체크



}

bool UEventTriggerManagerSubsystem::ShouldCreateSubsystem( UObject* Outer ) const
{
    const UGameInstance* pGameInstance = Cast<UGameInstance>( Outer );
    if( IsValid( pGameInstance ) == false )
        return false;

    UWorld* pWorld = pGameInstance->GetWorld();
    return IsValid( pWorld ) == true && pWorld->GetNetMode() == NM_DedicatedServer;
}

void UEventTriggerManagerSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
    Super::Initialize( Collection );

    UE_LOG( LogEventTriggerManagerSubsystem, Log, TEXT( "EventTriggerManagerSubsystem Initialized." ) );
}

void UEventTriggerManagerSubsystem::Deinitialize()
{
    Super::Deinitialize();

    UE_LOG( LogEventTriggerManagerSubsystem, Log, TEXT( "EventTriggerManagerSubsystem Deinitialized." ) );
}
