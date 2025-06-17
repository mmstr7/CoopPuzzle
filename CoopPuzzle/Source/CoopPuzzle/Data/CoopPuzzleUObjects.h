// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoopPuzzleUObjects.generated.h"

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UInventorySlotData : public UObject
{
	GENERATED_BODY()
	
public:
	void Init( int64 iItemUID ) { iItemUID = ItemUID; }

	UPROPERTY(BlueprintReadOnly)
	int64 ItemUID = -1;
};