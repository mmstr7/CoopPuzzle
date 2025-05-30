// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzleCharacter.generated.h"

class AEventTriggerObjectBase;
class ACoopPuzzlePlayerController;

UCLASS(Blueprintable)
class ACoopPuzzleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACoopPuzzleCharacter();

	const int64& GetPlayerUID() const { return m_iPlayerUID; }

protected:
	virtual void BeginPlay() override;
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

	int64 m_iPlayerUID = -1;
};

