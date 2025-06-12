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
#include "CoopPuzzle/Subsystem/WorldActorManagerSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzlePlayerState.h"
#include "Net/UnrealNetwork.h"

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
}

void ACoopPuzzleCharacter::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME( ACoopPuzzleCharacter, R_iPlayerUID );
}

void ACoopPuzzleCharacter::BeginPlay()
{
	Super::BeginPlay();

	if( GetNetMode() == NM_DedicatedServer )
	{
		UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
		if( IsValid( pGameInstance ) == false )
			return;

		R_iPlayerUID = pGameInstance->GenerateUID_DE();

		UWorldActorManagerSubsystem* pWorldActorManagerSubsystem = pGameInstance->GetSubsystem<UWorldActorManagerSubsystem>();
		if( IsValid( pWorldActorManagerSubsystem ) == true )
		{
			pWorldActorManagerSubsystem->RegisterPlayer( this );
		}
	}
}

void ACoopPuzzleCharacter::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

	if( GetNetMode() == NM_DedicatedServer )
	{
		if( IsValid( GetGameInstance() ) == false )
			return;
		
		UWorldActorManagerSubsystem* pWorldActorManagerSubsystem = GetGameInstance()->GetSubsystem<UWorldActorManagerSubsystem>();
		if( IsValid( pWorldActorManagerSubsystem ) == true )
		{
			pWorldActorManagerSubsystem->UnregisterPlayer( GetPlayerUID() );
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
		break;
	}
}
