// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "Net/UnrealNetwork.h"


AEventTriggerObjectBase::AEventTriggerObjectBase()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "DefaultMesh" ) );
	if( IsValid( DefaultMesh ) == true )
	{
		SetRootComponent( DefaultMesh );
	}

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerVolume" ) );
	if( IsValid( TriggerVolume ) == true )
	{
		TriggerVolume->SetupAttachment( RootComponent );
	}
}

void AEventTriggerObjectBase::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	//DOREPLIFETIME( AEventTriggerObjectBase, R_eTriggerState );

}

void AEventTriggerObjectBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if( IsNetMode( NM_DedicatedServer ) == true && IsValid( TriggerVolume ) == true )
	{
		TriggerVolume->OnComponentBeginOverlap.AddDynamic( this, &AEventTriggerObjectBase::OnTriggerVolumeBeginOverlap_DE );
		TriggerVolume->OnComponentEndOverlap.AddDynamic( this, &AEventTriggerObjectBase::OnTriggerVolumeEndOverlap_DE );
	}
}

void AEventTriggerObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	if( IsValid( GetGameInstance() ) == false )
		return;

	UDataTableSubsystem* pDataTableSubsystem = GetGameInstance()->GetSubsystem<UDataTableSubsystem>();
	checkf( IsValid( pDataTableSubsystem ) == true, TEXT( "DataTableSubsystem is not valid. Please check class." ) );

	m_pEventTriggerData = pDataTableSubsystem->GetDataRowOrNull<FEventTriggerDataRow>( EDataTableType::EventTrigger, EventTriggerID );
	checkf( m_pEventTriggerData != nullptr, TEXT( "EventTriggerID [%s] is not valid. Please check EventDataTable or [%s] class detail." ), *EventTriggerID.ToString(), *FString( GetClass()->GetName() ) );

	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		UEventTriggerManagerSubsystem* pEventTriggerSubsystem = GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>();
		checkf( IsValid( pEventTriggerSubsystem ) == true, TEXT( "EventTriggerSubsystem is not valid. Please check class." ) );
		
		pEventTriggerSubsystem->RegisterEventTrigger( GetEventTriggerID(), FOnEventTriggerCompleted::CreateUObject( this, &AEventTriggerObjectBase::OnTriggered_DE ) );
	}
}

void AEventTriggerObjectBase::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		UEventTriggerManagerSubsystem* pEventTriggerSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>() : nullptr;
		if( IsValid( pEventTriggerSubsystem ) == true )
		{
			pEventTriggerSubsystem->UnregisterEventTrigger( GetEventTriggerID() );
		}
	}
}

void AEventTriggerObjectBase::OnTriggerVolumeBeginOverlap_DE( class UPrimitiveComponent* pOverlappedComp, class AActor* pOtherActor, class UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	ACoopPuzzleCharacter* pPlayer = static_cast< ACoopPuzzleCharacter* >( pOtherActor );
	if( IsValid( pPlayer ) == false )
		return;

	UEventTriggerManagerSubsystem* pEventTriggerSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>() : nullptr;
	if( IsValid( pEventTriggerSubsystem ) == false )
		return;

	pEventTriggerSubsystem->LinkPlayerToEventTrigger( pPlayer->GetPlayerUID(), GetEventTriggerID() );

	if( m_pEventTriggerData->EventTriggerMode == EEventTriggerMode::InTriggerVolume )
		pEventTriggerSubsystem->TriggerEvent( pPlayer->GetPlayerUID(), EEventTriggerMode::InTriggerVolume );
}

void AEventTriggerObjectBase::OnTriggerVolumeEndOverlap_DE( class UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex )
{
	ACoopPuzzleCharacter* pPlayer = static_cast< ACoopPuzzleCharacter* >( pOtherActor );
	if( IsValid( pPlayer ) == false )
		return;

	UEventTriggerManagerSubsystem* pEventTriggerSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>() : nullptr;
	if( IsValid( pEventTriggerSubsystem ) == false )
		return;

	pEventTriggerSubsystem->UnlinkPlayerToEventTrigger( pPlayer->GetPlayerUID(), GetEventTriggerID() );
}

void AEventTriggerObjectBase::OnTriggered_DE( EEventTriggerResult eResult )
{
	checkf( IsNetMode( NM_DedicatedServer ) == true, TEXT( "Dedicated Server Only." ) );
	checkf( eResult != EEventTriggerResult::None , TEXT( "eResult is None. Please check call site." ) );

	CLIENT_OnTriggered( eResult );
}

void AEventTriggerObjectBase::CLIENT_OnTriggered_Implementation( EEventTriggerResult eResult )
{
}