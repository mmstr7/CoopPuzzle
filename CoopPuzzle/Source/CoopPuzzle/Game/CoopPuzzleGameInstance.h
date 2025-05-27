// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoopPuzzleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UCoopPuzzleGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
};
