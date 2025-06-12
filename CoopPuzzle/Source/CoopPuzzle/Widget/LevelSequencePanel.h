// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "LevelSequencePanel.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API ULevelSequencePanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY( EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* SkipBtn;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* WaitingText;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnPressedSkipBtn();

	void UpdateCurrentState( EProcessState eState );

	EProcessState m_eCurrentLevelSequenceState = EProcessState::None;
};
