// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoopPuzzleCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Player/CoopPuzzlePlayerController.h"
#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "CoopPuzzle/Player/CoopPuzzlePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "CoopPuzzle/Widget/ItemDropPad.h"
#include "CoopPuzzle/Subsystem/PlayerManagerSubsystem.h"

ACoopPuzzleCharacter::ACoopPuzzleCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	ItemDropPad = CreateDefaultSubobject<UWidgetComponent>( TEXT( "ItemDropPad" ) );
	ItemDropPad->SetupAttachment( RootComponent );
	ItemDropPad->SetWidgetClass( UItemDropPad::StaticClass() );
	ItemDropPad->SetWidgetSpace( EWidgetSpace::Screen );
	ItemDropPad->SetDrawAtDesiredSize( true );
}

void ACoopPuzzleCharacter::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( ACoopPuzzleCharacter, R_iPlayerUID );
}

void ACoopPuzzleCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
	if( IsValid( pGameInstance ) == false )
		return;

	if( GetNetMode() == NM_DedicatedServer )
	{
		// DE
		R_iPlayerUID = pGameInstance->GenerateUID_DE();

		UPlayerManagerSubsystem* pPlayerManagerSubsystem = pGameInstance->GetSubsystem<UPlayerManagerSubsystem>();
		if( IsValid( pPlayerManagerSubsystem ) == true )
		{
			pPlayerManagerSubsystem->RegisterPlayer( this );
		}
	}
	else
	{
		// CL
		if( IsValid( ItemDropPad ) == true )
		{
			UpdateDropPadAccountUID_CL();

			// 로컬 유저가 아닌 경우에만 아이템 드롭 패드 가시성 활성화
			ItemDropPad->SetVisibility( pGameInstance->IsLocalPlayer_CL( this ) == false );
		}
	}
}

void ACoopPuzzleCharacter::PossessedBy( AController* NewController )
{
	Super::PossessedBy( NewController );

	m_pPlayerController = static_cast< ACoopPuzzlePlayerController* >( NewController );
	if( IsValid( m_pPlayerController ) == false )
		return;

	if( m_pPlayerController->OnInputAction_DE.IsBound() == false )
	{
		m_pPlayerController->OnInputAction_DE.AddUObject( this, &ACoopPuzzleCharacter::OnKeyPressed_DE );
	}

	ACoopPuzzlePlayerState* pPlayerState = static_cast< ACoopPuzzlePlayerState* >( GetPlayerState() );
	if( IsValid( pPlayerState ) == true )
	{
		pPlayerState->BindEventDelegates_DE( GetPlayerUID() );
	}
}

void ACoopPuzzleCharacter::UnPossessed()
{
	Super::UnPossessed();

	if( IsValid( m_pPlayerController ) == false )
		return;

	m_pPlayerController->OnInputAction_DE.RemoveAll( this );
	m_pPlayerController = nullptr;

	ACoopPuzzlePlayerState* pPlayerState = static_cast< ACoopPuzzlePlayerState* >( GetPlayerState() );
	if( IsValid( pPlayerState ) == true )
	{
		pPlayerState->UnbindEventDelegates_DE( GetPlayerUID() );
	}
}

void ACoopPuzzleCharacter::OnKeyPressed_DE( EPlayerInputType ePlayerInputType ) const
{
	checkf( ePlayerInputType != EPlayerInputType::None, TEXT( "Invalid input: EPlayerInputType::None. Check input binding or call site." ) );

	switch( ePlayerInputType )
	{
	case EPlayerInputType::Interact:
	{
		UEventTriggerManagerSubsystem* pTriggerManagerSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UEventTriggerManagerSubsystem>() : nullptr;
		if( IsValid( pTriggerManagerSubsystem ) == false )
			return;

		pTriggerManagerSubsystem->TriggerManualEvent( GetPlayerUID(), EManualTriggerMode::InputInteractKey );
	} break;
	default:
		// TODO : checkf 추가
		break;
	}
}

void ACoopPuzzleCharacter::OnRep_PlayerUID()
{
	if( R_iPlayerUID == INVALID_PLAYER_UID )
		return;

	UpdateDropPadAccountUID_CL();
}

void ACoopPuzzleCharacter::UpdateDropPadAccountUID_CL()
{
	if( IsValid( ItemDropPad ) == false )
		return;

	UItemDropPad* pItemDropPad = Cast<UItemDropPad>( ItemDropPad->GetWidget() );
	if( IsValid( pItemDropPad ) == false )
		return;

	pItemDropPad->SetOwnerPlayerUID( R_iPlayerUID );
}
