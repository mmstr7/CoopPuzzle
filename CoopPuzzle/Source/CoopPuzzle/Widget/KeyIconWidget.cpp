// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/KeyIconWidget.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "Components/TextBlock.h"

void UKeyIconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnShowPlayerInputIcon_ToClient.FindOrAdd( 0 ).AddUObject( this, &UKeyIconWidget::UpdateKeyIconWidget );
	}

	// юс╫ц
	m_mapInputKeyText = { { EPlayerInputType::Interact, FText::FromString( "F" ) } };

	UpdateKeyIconWidget( EPlayerInputType::Interact, false );
}

void UKeyIconWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnShowPlayerInputIcon_ToClient.Remove( 0 );
	}
}

void UKeyIconWidget::UpdateKeyIconWidget( EPlayerInputType eInputType, bool bShow )
{
	if( IsValid( InputKeyText ) == false )
		return;

	InputKeyText->SetVisibility( bShow == true ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed );

	if( bShow == true )
	{
		if( m_eLastInputType != eInputType )
		{
			const FText* pKeyText = m_mapInputKeyText.Find( eInputType );
			if( pKeyText == nullptr || pKeyText->IsEmpty() == true )
				return;

			FText FormatText = FText::FromStringTable( FName( TEXT( "/Game/TopDown/DataTable/GlobalStringTable" ) ), TEXT( "Widget_KeyIconWidget_InputKeyText" ) );
			InputKeyText->SetText( FText::Format( FormatText, FFormatNamedArguments{ { TEXT( "Key" ), *pKeyText } } ) );
			m_eLastInputType = eInputType;
		}

		InputKeyText->SetVisibility( ESlateVisibility::HitTestInvisible );
	}
}
