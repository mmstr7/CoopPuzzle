// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/NotificationText.h"
#include "NotificationText.h"
#include "Components/TextBlock.h"

void UNotificationText::AddNotification( const FText& Message )
{
	if( IsValid( NotificationText ) == false )
		return;

	NotificationText->SetText( Message );
	NotificationText->SetVisibility( ESlateVisibility::HitTestInvisible );
	bNotificationEnded = false;
	LastNotificationTime = FDateTime::UtcNow();
}

void UNotificationText::NativePreConstruct()
{
	Super::NativePreConstruct();

	if( IsValid( NotificationText ) == true )
	{
		NotificationText->SetFont( NotificationFont );
		NotificationText->SetColorAndOpacity( NotificationColor );
	}
}

void UNotificationText::NativeConstruct()
{
	Super::NativeConstruct();

	if( IsValid( NotificationText ) == false )
		return;

	NotificationText->SetVisibility( ESlateVisibility::Collapsed );
}

void UNotificationText::NativeTick( const FGeometry& MyGeometry, float InDeltaTime )
{
	Super::NativeTick( MyGeometry, InDeltaTime );

	if( bNotificationEnded == false && FDateTime::UtcNow() >= ( LastNotificationTime + FTimespan::FromSeconds( NotificationDuration ) ) )
	{
		NotificationText->SetVisibility( ESlateVisibility::Collapsed );
		bNotificationEnded = true;
	}
}
