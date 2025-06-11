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
}

void AEventTriggerObjectBase::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AEventTriggerObjectBase, R_eTriggerState );
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

	if( IsValid( TriggerVolume ) == true )
	{
		// DE이고 수동 트리거라면 오버랩 이벤트 켜기
		bool bEnableOverlap = IsNetMode( NM_DedicatedServer ) == true && m_pEventTriggerData->AutoTriggerOnCondition == false;

		TriggerVolume->SetGenerateOverlapEvents( bEnableOverlap );
		if( bEnableOverlap == true )
		{
			TriggerVolume->OnComponentBeginOverlap.AddDynamic( this, &AEventTriggerObjectBase::OnTriggerVolumeBeginOverlap_DE );
			TriggerVolume->OnComponentEndOverlap.AddDynamic( this, &AEventTriggerObjectBase::OnTriggerVolumeEndOverlap_DE );
		}

		//TODO : 델리게이트 해제 로직 추가
	}

	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		SetTriggerState_DE( m_pEventTriggerData->DefaultState );

		UEventTriggerManagerSubsystem* pEventTriggerSubsystem = GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>();
		if( IsValid( pEventTriggerSubsystem ) == true )
		{
			pEventTriggerSubsystem->RegisterEventTrigger( this, FOnEventTriggerCompleted::CreateUObject( this, &AEventTriggerObjectBase::OnTriggered_DE ) );
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
			pEventTriggerSubsystem->UnregisterEventTrigger( GetEventTriggerID() );
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
	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		if( R_eTriggerState == EEventTriggerState::Triggered )
		{
			UEventTriggerManagerSubsystem* pEventTriggerSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>() : nullptr;
			if( IsValid( pEventTriggerSubsystem ) == true )
			{
				pEventTriggerSubsystem->TriggerAutoEvent( 0, EEventTriggerCondition::OtherTrigger_Triggered, EventTriggerID );
			}
		}
	}
	else
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
		if( m_pEventTriggerData->ManualTriggerMode == EManualTriggerMode::InTriggerVolume_Toggle )
		{
			SetTriggerState_DE( R_eTriggerState == EEventTriggerState::Triggered ? EEventTriggerState::Enabled : EEventTriggerState::Triggered );
			break;
		}

		SetTriggerState_DE( EEventTriggerState::Triggered );
	} break;
	case EEventTriggerResult::Failed:
	{
		if( m_pEventTriggerData->ManualTriggerMode == EManualTriggerMode::InTriggerVolume_Stay )
		{
			SetTriggerState_DE( EEventTriggerState::Enabled );
			break;
		}

		// 아무 일도 일어나지 않음
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
