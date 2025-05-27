// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopPuzzle/Utility/CoopPuzzleEnums.h"
#include "EventTriggerObjectBase.generated.h"

UCLASS()
class COOPPUZZLE_API AEventTriggerObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEventTriggerObjectBase();

	FORCEINLINE const FName& GetEventTriggerID() { return EventTriggerID; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setting)
	FName EventTriggerID = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* TriggerVolume = nullptr;
};
