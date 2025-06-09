// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UTileView;
class UButton;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UFUNCTION()
	void UpdateInventoryPanel();

private:
	UPROPERTY( meta = (BindWidget))
	UTileView* ItemList;

	int32 m_iDefaultSlotCount = 4;
};
