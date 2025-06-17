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
	virtual void PossessedBy( AController* NewController ) override;
	virtual void UnPossessed() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* ItemDropPad = nullptr;

private:
	UFUNCTION()
	void OnKeyPressed_DE( EPlayerInputType ePlayerInputType ) const;

	UFUNCTION()
	void OnRep_PlayerUID();

	// ItemDropPad 위젯에 해당 유저의 AccountUID를 세팅하는 함수입니다.
	// CL에서는 AccountUID의 동기화와 ItemDropPad 생성 순서가 보장되지 않기 때문에
	// BeginPlay()와 OnRep_PlayerUID() 양쪽에서 각각 한 번씩 호출합니다.
	void UpdateDropPadAccountUID_CL();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY()
	ACoopPuzzlePlayerController* m_pPlayerController = nullptr;

	UPROPERTY()
	AEventTriggerObjectBase* m_pInteractableEventTrigger = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerUID)
	int64 R_iPlayerUID = INVALID_PLAYER_UID;
};

