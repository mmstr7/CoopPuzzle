// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "EventTriggerMovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API AEventTriggerMovingPlatform : public AEventTriggerObjectBase
{
	GENERATED_BODY()

public:
	AEventTriggerMovingPlatform();
	
protected:
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MovingPlatformMesh = nullptr;

	UPROPERTY( EditDefaultsOnly )
	float Speed = 0.1f;

	UPROPERTY( EditDefaultsOnly )
	FVector StartRelativeLoc = FVector::Zero();

	UPROPERTY( EditDefaultsOnly )
	FVector EndRelativeLoc = FVector::Zero();

private:
	// �ʱ� ���� �ÿ��� ����ȭ. ����� ������ ������ ������ (-1.0f ~ 1.0f)
	UPROPERTY( Replicated )
	float R_fMovePercent = 0.f;
};
