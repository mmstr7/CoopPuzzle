// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoopPuzzle/Widget/TooltipBase.h"
#include "ItemTooltip.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UItemTooltip : public UTooltipBase
{
	GENERATED_BODY()
	
public:
	virtual void SetupTooltipWidget( const FName& ItemID ) override;
	
private:
	UPROPERTY( meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY( meta = (BindWidget))
	UTextBlock* Description;

	FName m_CachedItemID = FName();
};
