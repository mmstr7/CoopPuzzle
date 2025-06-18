// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/NotificationText.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UNotificationText::AddNotification( const FText& Message )
{
	if( IsValid( VerticalBox ) == false )
		return;

	UTextBlock* pNewTextBlock = NewObject<UTextBlock>( this );
	if( IsValid( pNewTextBlock ) == false )
		return;

	pNewTextBlock->SetText( Message );
	pNewTextBlock->SetVisibility( ESlateVisibility::HitTestInvisible );
	pNewTextBlock->SetFont( NotificationFont );
	pNewTextBlock->SetColorAndOpacity( NotificationColor );
	pNewTextBlock->SetJustification( ETextJustify::Type::Center );

	m_queueExpireTimes.Enqueue( FDateTime::UtcNow() + FTimespan::FromSeconds( NotificationDuration ) );

	VerticalBox->AddChildToVerticalBox( pNewTextBlock );
}

void UNotificationText::NativeConstruct()
{
	Super::NativeConstruct();

	if( IsValid( VerticalBox ) == true )
	{
		VerticalBox->ClearChildren();
	}
}

void UNotificationText::NativeTick( const FGeometry& MyGeometry, float InDeltaTime )
{
	Super::NativeTick( MyGeometry, InDeltaTime );

	if( m_queueExpireTimes.IsEmpty() == false && FDateTime::UtcNow() >= *m_queueExpireTimes.Peek() )
	{
		m_queueExpireTimes.Pop();

		if( IsValid( VerticalBox ) == false )
			return;
		
		VerticalBox->RemoveChildAt( 0 );
	}
}
