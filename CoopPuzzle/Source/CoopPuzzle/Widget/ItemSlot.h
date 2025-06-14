// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "ItemSlot.generated.h"

class UTooltipBase;
class UTextBlock;
class UImage;
class UOverlay;

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

	// BP의 Detail 툴팁 위젯 세팅에 해당 함수를 바인딩 합니다.
	UFUNCTION( BlueprintCallable )
	UTooltipBase* GetItemTooltipWidget();
	
private:
	UPROPERTY( meta = (BindWidget))
	UTextBlock* ItemCountText;
	
	UPROPERTY( meta = (BindWidget))
	UImage* ItemIconImage;

	int64 m_iCachedItemUID = -1;
	FName m_CachedItemID = FName();
};
