// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "KeyIconWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UKeyIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	UFUNCTION()
	void UpdateKeyIconWidget( EPlayerInputType eInputType, bool bShow );

	private:
	UPROPERTY( meta = (BindWidget))
	UTextBlock* InputKeyText;

	EPlayerInputType m_eLastInputType = EPlayerInputType::None;

	TMap<EPlayerInputType, FText> m_mapInputKeyText;
};
