// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/CoopPuzzleMainHUD.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Widget/NotificationText.h"

void UCoopPuzzleMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnShowGlobalNotification.AddUObject( this, &UCoopPuzzleMainHUD::ShowGlobalNotification );
		pWidgetDelegateSubsystem->OnShowLocalNotification.FindOrAdd( 0 ).AddUObject( this, &UCoopPuzzleMainHUD::ShowLocalNotification );
	}
}

void UCoopPuzzleMainHUD::NativeDestruct()
{
	Super::NativeDestruct();

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnShowGlobalNotification.RemoveAll( this );
		pWidgetDelegateSubsystem->OnShowLocalNotification.Remove( 0 );
	}
}

void UCoopPuzzleMainHUD::ShowGlobalNotification( const FText& Message )
{
	if( IsValid( GlobalNotification ) == false )
		return;

	GlobalNotification->AddNotification( Message );
}

void UCoopPuzzleMainHUD::ShowLocalNotification( const FText& Message )
{
	if( IsValid( LocalNotification ) == false )
		return;

	LocalNotification->AddNotification( Message );
}
