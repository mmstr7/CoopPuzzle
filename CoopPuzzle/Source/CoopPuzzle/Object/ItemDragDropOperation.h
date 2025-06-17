// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	int64 ItemUID = INVALID_ITEM_UID;
};
