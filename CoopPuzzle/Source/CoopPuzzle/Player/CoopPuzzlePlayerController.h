// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Delegates/DelegateCombinations.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzlePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionInstance;

DECLARE_MULTICAST_DELEGATE_OneParam( FOnInputAction_DE, EPlayerInputType );
DECLARE_LOG_CATEGORY_EXTERN( LogCoopPuzzleController, Log, All );

UCLASS()
class ACoopPuzzlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACoopPuzzlePlayerController();

	FOnInputAction_DE OnInputAction_DE;

protected:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext = nullptr;

private:
	void OnMove( const FInputActionInstance& Instance );

	UFUNCTION( Server, Reliable )
	void SERVER_OnInteract( const FInputActionInstance& Instance );
	FORCEINLINE void SERVER_OnInteract_Implementation( const FInputActionInstance& Instance ) { OnInputAction_DE.Broadcast( EPlayerInputType::Interact ); }
};


