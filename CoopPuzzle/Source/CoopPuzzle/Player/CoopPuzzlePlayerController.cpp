// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoopPuzzlePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

DEFINE_LOG_CATEGORY( LogCoopPuzzleController );

ACoopPuzzlePlayerController::ACoopPuzzlePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ACoopPuzzlePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACoopPuzzlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	checkf( IsValid( DefaultMappingContext ) == true, TEXT( "DefaultMappingContext is not set. Did you forget to assign it?" ) );
	checkf( IsValid( MoveAction ) == true, TEXT( "MoveAction is not set. Did you forget to assign it?" ) );

	UEnhancedInputLocalPlayerSubsystem* pInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( GetLocalPlayer() );
	if( IsValid( pInputLocalPlayerSubsystem ) == true )
	{
		pInputLocalPlayerSubsystem->AddMappingContext( DefaultMappingContext, 0 );
	}

	UEnhancedInputComponent* pEnhancedInputComponent = Cast<UEnhancedInputComponent>( InputComponent );
	if( IsValid( pEnhancedInputComponent ) == true )
	{
		pEnhancedInputComponent->BindAction( MoveAction, ETriggerEvent::Triggered, this, &ACoopPuzzlePlayerController::OnMove );
	}
}

void ACoopPuzzlePlayerController::OnMove( const FInputActionInstance& Instance )
{
	APawn* pLocalPlayer = GetPawn();
	if( IsValid( pLocalPlayer ) == false )
	{
		UE_LOG( LogCoopPuzzleController, Error, TEXT( "ACoopPuzzlePlayerController::OnMove() : LocalPlayer is not valid." ) );
		return;
	}

	const FVector2D Value = Instance.GetValue().Get<FVector2D>();

	if( FMath::IsNearlyZero( Value.X ) == false )
		pLocalPlayer->AddMovementInput( FVector( 0, 1, 0 ), Value.X );

	if( FMath::IsNearlyZero( Value.Y ) == false )
		pLocalPlayer->AddMovementInput( FVector( 1, 0, 0 ), Value.Y );
}