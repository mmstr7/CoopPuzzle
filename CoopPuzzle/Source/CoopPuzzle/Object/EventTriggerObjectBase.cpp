// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "Components/BoxComponent.h"


AEventTriggerObjectBase::AEventTriggerObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerVolume" ) );
}

void AEventTriggerObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	UDataTableSubsystem* pDataTableSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UDataTableSubsystem>() : nullptr;
	if( IsValid( pDataTableSubsystem ) == true )
	{
		m_pEventData = pDataTableSubsystem->GetDataRowOrNull<FEventDataRow>( EDataTableType::Event, EventTriggerID );
		checkf( m_pEventData != nullptr, TEXT( "EventTriggerID [%s] is not valid. Please check EventDataTable or [%s] class detail." ), *FString( GetClass()->GetName() ), *EventTriggerID.ToString() );
	}
}
