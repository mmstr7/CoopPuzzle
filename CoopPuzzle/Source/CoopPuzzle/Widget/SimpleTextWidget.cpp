// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/SimpleTextWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

void USimpleTextWidget::SetText( const FText& Text )
{
	if( IsValid( TextBlock ) == false )
		return;

	TextBlock->SetText( Text );	
}