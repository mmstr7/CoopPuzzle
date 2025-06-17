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

	// ItemDropPad ������ �ش� ������ AccountUID�� �����ϴ� �Լ��Դϴ�.
	// CL������ AccountUID�� ����ȭ�� ItemDropPad ���� ������ ������� �ʱ� ������
	// BeginPlay()�� OnRep_PlayerUID() ���ʿ��� ���� �� ���� ȣ���մϴ�.
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

