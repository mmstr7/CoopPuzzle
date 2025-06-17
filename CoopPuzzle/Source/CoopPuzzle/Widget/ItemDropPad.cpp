// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/ItemDropPad.h"
#include "CoopPuzzle/Object/ItemDragDropOperation.h"
#include "Components/TextBlock.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"

void UItemDropPad::NativeConstruct()
{
	Super::NativeConstruct();

	ShowHoverTooltipTextBlock( false );
}

void UItemDropPad::NativeOnDragEnter( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )
{
	Super::NativeOnDragEnter( InGeometry, InDragDropEvent, InOperation );

	UItemDragDropOperation* pItemDropOperation = Cast<UItemDragDropOperation>( InOperation );
	if( IsValid( pItemDropOperation ) == false )
		return;

	ShowHoverTooltipTextBlock( true );
}

void UItemDropPad::NativeOnDragLeave( const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )
{
	Super::NativeOnDragLeave( InDragDropEvent, InOperation );

	ShowHoverTooltipTextBlock( false );
}

bool UItemDropPad::NativeOnDrop( const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation )
{
	Super::NativeOnDrop( InGeometry, InDragDropEvent, InOperation );

	ShowHoverTooltipTextBlock( false );

	if( m_iOwnerPlayerUID == INVALID_PLAYER_UID )
		return false;

	UItemDragDropOperation* pItemDropOperation = Cast<UItemDragDropOperation>( InOperation );
	if( IsValid( pItemDropOperation ) == false )
		return false;

	UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
	if( IsValid( pItemSubsystem ) == false )
		return false;

	pItemSubsystem->TryGiveItemToPlayer_CL( m_iOwnerPlayerUID, pItemDropOperation->ItemUID, 1 );

	return true;
}

void UItemDropPad::ShowHoverTooltipTextBlock( bool bShow )
{
	if( IsValid( HoverTooltipTextBlock ) == false )
		return;

	HoverTooltipTextBlock->SetVisibility( bShow == true ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed );
}
