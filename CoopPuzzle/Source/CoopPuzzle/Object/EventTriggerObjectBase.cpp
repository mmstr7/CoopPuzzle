// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "CoopPuzzle/Widget/SimpleTextWidget.h"


AEventTriggerObjectBase::AEventTriggerObjectBase()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "DefaultMesh" ) );
	SetRootComponent( DefaultMesh );

	EnableIndicator = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "EnableIndicator" ) );
	EnableIndicator->SetupAttachment( RootComponent );

	TriggerVolume_DE = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerVolume_DE" ) );
	TriggerVolume_DE->SetupAttachment( RootComponent );

	HintTextVolume_CL = CreateDefaultSubobject<UBoxComponent>( TEXT( "IndicatorTextVolume_CL" ) );
	HintTextVolume_CL->SetupAttachment( RootComponent );

	HintTextWidget = CreateDefaultSubobject<UWidgetComponent>( TEXT( "IndicatorText" ) );
	HintTextWidget->SetupAttachment( RootComponent );
	HintTextWidget->SetWidgetClass( USimpleTextWidget::StaticClass() );
	HintTextWidget->SetWidgetSpace( EWidgetSpace::Screen );
	HintTextWidget->SetDrawAtDesiredSize( true );
}

void AEventTriggerObjectBase::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( AEventTriggerObjectBase, R_eTriggerState );
}

void AEventTriggerObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
	if( IsValid( pGameInstance ) == false )
		return;

	UDataTableSubsystem* pDataTableSubsystem = pGameInstance->GetSubsystem<UDataTableSubsystem>();
	checkf( IsValid( pDataTableSubsystem ) == true, TEXT( "DataTableSubsystem is not valid. Please check class." ) );

	m_pEventTriggerData = pDataTableSubsystem->GetDataRowOrNull<FEventTriggerDataRow>( EDataTableType::EventTrigger, EventTriggerID );
	checkf( m_pEventTriggerData != nullptr, TEXT( "EventTriggerID [%s] is not valid. Please check EventDataTable or [%s] class detail." ), *EventTriggerID.ToString(), *FString( GetClass()->GetName() ) );

	// DE일 경우 관련 서브시스템에 데이터 등록
	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		m_iEventTriggerUID_DE = pGameInstance->GenerateUID_DE();

		SetTriggerState_DE( m_pEventTriggerData->DefaultState );

		UEventTriggerManagerSubsystem* pEventTriggerSubsystem = pGameInstance->GetSubsystem<UEventTriggerManagerSubsystem>();
		if( IsValid( pEventTriggerSubsystem ) == true )
		{
			pEventTriggerSubsystem->RegisterEventTrigger( this, FOnEventTriggerCompleted::CreateUObject( this, &AEventTriggerObjectBase::OnTriggered_DE ) );
		}
	}

	// DE이고 수동 트리거라면 오버랩 이벤트 켜기
	if( IsValid( TriggerVolume_DE ) == true )
	{
		bool bEnableOverlap = IsNetMode( NM_DedicatedServer ) == true && m_pEventTriggerData->AutoTriggerOnCondition == false;

		TriggerVolume_DE->SetGenerateOverlapEvents( bEnableOverlap );
		if( bEnableOverlap == true )
		{
			TriggerVolume_DE->OnComponentBeginOverlap.AddDynamic( this, &AEventTriggerObjectBase::OnTriggerVolumeBeginOverlap_DE );
			TriggerVolume_DE->OnComponentEndOverlap.AddDynamic( this, &AEventTriggerObjectBase::OnTriggerVolumeEndOverlap_DE );
		}
	}

	// CL이고 인디케이터 텍스트가 세팅되어 있다면 오버랩 이벤트 켜기
	if( IsValid( HintTextVolume_CL ) == true )
	{
		bool bEnableOverlap = IsNetMode( NM_Client ) == true && m_pEventTriggerData->HintText.IsEmpty() == false;

		HintTextVolume_CL->SetGenerateOverlapEvents( bEnableOverlap );

		if( bEnableOverlap == true )
		{
			HintTextVolume_CL->OnComponentBeginOverlap.AddDynamic( this, &AEventTriggerObjectBase::OnIndicatorTextVolumeBeginOverlap_CL );
			HintTextVolume_CL->OnComponentEndOverlap.AddDynamic( this, &AEventTriggerObjectBase::OnIndicatorTextVolumeEndOverlap_CL );
		}
	}
}

void AEventTriggerObjectBase::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		UCoopPuzzleGameInstance* pGameInstance = Cast< UCoopPuzzleGameInstance>( GetGameInstance() );
		if( IsValid( pGameInstance ) == false )
			return;

		m_iEventTriggerUID_DE = pGameInstance->GenerateUID_DE();

		UEventTriggerManagerSubsystem* pEventTriggerSubsystem = GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>();
		if( IsValid( pEventTriggerSubsystem ) == true )
		{
			pEventTriggerSubsystem->UnregisterEventTrigger( m_iEventTriggerUID_DE );
		}
	}

	if( IsValid( TriggerVolume_DE ) == true )
	{
		TriggerVolume_DE->OnComponentBeginOverlap.RemoveAll( this );
		TriggerVolume_DE->OnComponentEndOverlap.RemoveAll( this );
	}

	if( IsValid( HintTextVolume_CL ) == true )
	{
		HintTextVolume_CL->OnComponentBeginOverlap.RemoveAll( this );
		HintTextVolume_CL->OnComponentEndOverlap.RemoveAll( this );
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

		UpdateHintTextWidget();
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

	pEventTriggerSubsystem->LinkPlayerToEventTrigger( pPlayer->GetPlayerUID(), GetEventTriggerUID_DE() );
}

void AEventTriggerObjectBase::OnTriggerVolumeEndOverlap_DE( class UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex )
{
	ACoopPuzzleCharacter* pPlayer = static_cast< ACoopPuzzleCharacter* >( pOtherActor );
	if( IsValid( pPlayer ) == false )
		return;

	UEventTriggerManagerSubsystem* pEventTriggerSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>() : nullptr;
	if( IsValid( pEventTriggerSubsystem ) == false )
		return;

	pEventTriggerSubsystem->UnlinkPlayerToEventTrigger( pPlayer->GetPlayerUID(), GetEventTriggerUID_DE() );
}

void AEventTriggerObjectBase::OnIndicatorTextVolumeBeginOverlap_CL( UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
	if( IsValid( pGameInstance ) == false || pGameInstance->IsLocalPlayer_CL( Cast<ACoopPuzzleCharacter>( pOtherActor ) ) == false )
		return;

	m_bIHintTextVolumeOverlapped_CL = true;
	UpdateHintTextWidget();
}

void AEventTriggerObjectBase::OnIndicatorTextVolumeEndOverlap_CL( UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex )
{
	UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
	if( IsValid( pGameInstance ) == false || pGameInstance->IsLocalPlayer_CL( Cast<ACoopPuzzleCharacter>( pOtherActor ) ) == false )
		return;

	m_bIHintTextVolumeOverlapped_CL = false;
	UpdateHintTextWidget();
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

void AEventTriggerObjectBase::UpdateHintTextWidget()
{
	USimpleTextWidget* pHintTextWidget = IsValid( HintTextWidget ) == true ? Cast<USimpleTextWidget>( HintTextWidget->GetWidget() ) : nullptr;
	if( IsValid( pHintTextWidget ) == false )
		return;

	bool bShow = m_bIHintTextVolumeOverlapped_CL == true && R_eTriggerState == EEventTriggerState::Enabled;

	pHintTextWidget->SetVisibility( bShow == true ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed );
	if( bShow == true )
	{
		pHintTextWidget->SetText( m_pEventTriggerData->HintText );
	}
}
