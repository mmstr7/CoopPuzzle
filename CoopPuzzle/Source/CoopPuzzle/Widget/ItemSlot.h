// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ItemSlot.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UItemSlot : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	void SetItemInfo( int64 iItemUID, bool bIsInit );

protected:
	virtual void NativeOnListItemObjectSet( UObject* ListItemObject ) override;
	
private:
	UPROPERTY( meta = (BindWidget))
	UTextBlock* ItemCountText;
	
	UPROPERTY( meta = (BindWidget))
	UImage* ItemIconImage;

	int64 m_iCachedItemUID = -1;
};
