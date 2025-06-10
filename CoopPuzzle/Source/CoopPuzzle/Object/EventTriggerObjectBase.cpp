// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "CoopPuzzle/Subsystem/WorldActorManagerSubsystem.h"


AEventTriggerObjectBase::AEventTriggerObjectBase()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "DefaultMesh" ) );
	SetRootComponent( DefaultMesh );

	EnableIndicator = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "EnableIndicator" ) );
	EnableIndicator->SetupAttachment( RootComponent );

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerVolume" ) );
	TriggerVolume->SetupAttachment( RootComponent );

	ConditionVolume = CreateDefaultSubobject<UBoxComponent>( TEXT( "ConditionVolume" ) );
	ConditionVolume->SetupAttachment( RootComponent );
}

bool AEventTriggerObjectBase::IsConditionOverlappingPlayer( const ACoopPuzzleCharacter* pPlayer ) const
{
	return IsValid( ConditionVolume ) == true && ConditionVolume->IsOverlappingActor( pPlayer ) == true;
}

void AEventTriggerObjectBase::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AEventTriggerObjectBase, R_eTriggerState );
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
		SetTriggerState_DE( m_pEventTriggerData->DefaultState );

		UEventTriggerManagerSubsystem* pEventTriggerSubsystem = GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>();
		if( IsValid( pEventTriggerSubsystem ) == true )
		{
			pEventTriggerSubsystem->RegisterEventTriggerHandle( this, FOnEventTriggerCompleted::CreateUObject( this, &AEventTriggerObjectBase::OnTriggered_DE ) );
		}

		UWorldActorManagerSubsystem* pWorldActorManagerSubsystem = GetGameInstance()->GetSubsystem<UWorldActorManagerSubsystem>();
		if( IsValid( pWorldActorManagerSubsystem ) == true )
		{
			pWorldActorManagerSubsystem->RegisterEventTrigger( this );
		}
	}
}

void AEventTriggerObjectBase::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		if( IsValid( GetGameInstance() ) == false )
			return;

		UEventTriggerManagerSubsystem* pEventTriggerSubsystem = GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>();
		if( IsValid( pEventTriggerSubsystem ) == true )
		{
			pEventTriggerSubsystem->UnregisterEventTriggerHandle( GetEventTriggerID() );
		}

		UWorldActorManagerSubsystem* pWorldActorManagerSubsystem = GetGameInstance()->GetSubsystem<UWorldActorManagerSubsystem>();
		if( IsValid( pWorldActorManagerSubsystem ) == true )
		{
			pWorldActorManagerSubsystem->UnregisterEventTrigger( GetEventTriggerID() );
		}
	}
}

void AEventTriggerObjectBase::OnRep_TriggerState_Implementation()
{
	if( IsNetMode( NM_DedicatedServer ) == false )
	{
		if( IsValid( EnableIndicator ) == true )
		{
			EnableIndicator->SetVisibility( R_eTriggerState == EEventTriggerState::Enabled );
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

	switch( eResult )
	{
	case EEventTriggerResult::Success:
	{
		SetTriggerState_DE( EEventTriggerState::Triggered );
	} break;
	case EEventTriggerResult::Failed:
	{
		// 아무 일도 일어나지 않음
	} break;
	case EEventTriggerResult::Reset:
	{
		SetTriggerState_DE( m_pEventTriggerData->DefaultState );
	} break;
	default:
	{
		// TODO: 올바르지 않은 타입이라고 checkf 추가
	} break;
	}
}

void AEventTriggerObjectBase::SetTriggerState_DE( EEventTriggerState eTriggerState )
{
	checkf( IsNetMode( NM_DedicatedServer ) == true, TEXT( "Dedicated Server Only." ) );

	if( eTriggerState == R_eTriggerState )
		return;

	R_eTriggerState = eTriggerState;
	OnRep_TriggerState();
}
