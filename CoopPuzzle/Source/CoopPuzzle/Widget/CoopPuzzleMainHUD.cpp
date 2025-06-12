// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/CoopPuzzleMainHUD.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Widget/NotificationText.h"
#include "CoopPuzzle/Subsystem/LevelSequenceSubsystem.h"
#include "Components/Button.h"

void UCoopPuzzleMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if( IsValid( GetGameInstance() ) == false )
		return;

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>();
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnShowGlobalNotification_ToClient.AddUObject( this, &UCoopPuzzleMainHUD::ShowGlobalNotification );
		pWidgetDelegateSubsystem->OnShowLocalNotification_ToClient.FindOrAdd( 0 ).AddUObject( this, &UCoopPuzzleMainHUD::ShowLocalNotification );
	}
}

void UCoopPuzzleMainHUD::NativeDestruct()
{
	Super::NativeDestruct();

	if( IsValid( GetGameInstance() ) == false )
		return;

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>();
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnShowGlobalNotification_ToClient.RemoveAll( this );
		pWidgetDelegateSubsystem->OnShowLocalNotification_ToClient.Remove( 0 );
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
