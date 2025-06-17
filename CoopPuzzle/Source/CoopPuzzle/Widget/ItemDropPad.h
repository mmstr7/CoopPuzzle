// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"
#include "ItemDropPad.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UItemDropPad : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwnerPlayerUID( int64 iPlayerUID ) { m_iOwnerPlayerUID = iPlayerUID; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnDragEnter( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	virtual void NativeOnDragLeave( const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;
	virtual bool NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation ) override;

private:
	void ShowHoverTooltipTextBlock( bool bShow );

	UPROPERTY( meta = (BindWidget))
	UTextBlock* HoverTooltipTextBlock;

	int64 m_iOwnerPlayerUID = INVALID_PLAYER_UID;
};
