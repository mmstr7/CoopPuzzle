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
		pWidgetDelegateSubsystem->OnShowGlobalNotification.AddDynamic( this, &UCoopPuzzleMainHUD::ShowGlobalNotification );
		pWidgetDelegateSubsystem->OnShowLocalNotification.AddDynamic( this, &UCoopPuzzleMainHUD::ShowLocalNotification );
	}
}

void UCoopPuzzleMainHUD::ShowGlobalNotification( const FText& Message )
{
	if( IsValid( GlobalNotification ) == false )
		return;

	GlobalNotification->AddNotification( Message );
}

void UCoopPuzzleMainHUD::ShowLocalNotification( const int64& iPlayerUID, const FText& Message )
{
	if( IsValid( LocalNotification ) == false )
		return;

	LocalNotification->AddNotification( Message );
}
