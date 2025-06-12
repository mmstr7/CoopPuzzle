// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"
#include "CoopPuzzleCharacter.generated.h"

class AEventTriggerObjectBase;
class ACoopPuzzlePlayerController;

UCLASS(Blueprintable)
class ACoopPuzzleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACoopPuzzleCharacter();

	const int64& GetPlayerUID() const { return R_iPlayerUID; }

protected:
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;
	virtual void PossessedBy( AController* NewController ) override;
	virtual void UnPossessed() override;

private:
	UFUNCTION()
	void OnKeyPressed_DE( EPlayerInputType ePlayerInputType ) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY()
	ACoopPuzzlePlayerController* m_pPlayerController = nullptr;

	UPROPERTY()
	AEventTriggerObjectBase* m_pInteractableEventTrigger = nullptr;

	UPROPERTY(Replicated)
	int64 R_iPlayerUID = INVALID_PLAYER_UID;
};

