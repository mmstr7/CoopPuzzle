// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleTextWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API USimpleTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetText( const FText& Text );

private:
	UPROPERTY( meta = (BindWidget))
	UTextBlock* TextBlock;
};
