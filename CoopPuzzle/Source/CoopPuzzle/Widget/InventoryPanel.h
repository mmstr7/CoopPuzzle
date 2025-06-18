// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "InventoryPanel.generated.h"

class UTileView;
class UNotificationText;

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
	
private:
	UFUNCTION()
	void UpdateInventoryPanel();

	UFUNCTION()
	void LevelSequenceStateChanged( EProcessState eState );

	UFUNCTION()
	void AddItemNotifications( const TArray<FItemNotifyInfo>& arrNotifications );

	UPROPERTY( meta = (BindWidget))
	UNotificationText* ItemUpdateNotification;

	UPROPERTY( meta = (BindWidget))
	UTileView* ItemList;

	int32 m_iDefaultSlotCount = 4;
};
