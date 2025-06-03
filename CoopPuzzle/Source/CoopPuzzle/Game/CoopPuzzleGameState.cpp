// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Game/CoopPuzzleGameState.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "Blueprint/UserWidget.h"

void ACoopPuzzleGameState::BeginPlay()
{
	Super::BeginPlay();

	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		// WidgetDelegateSubsystem 헬퍼 함수 바인딩
		UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
		if( IsValid( pWidgetDelegateSubsystem ) == true )
		{
			pWidgetDelegateSubsystem->OnShowGlobalNotification.AddDynamic( this, &ACoopPuzzleGameState::MULTI_OnShowGlobalNotification );
		}
	}
	else
	{
		if( IsValid( MainHUD ) == true )
		{
			MainHUD->AddToViewport( 0 );
		}
	}
}

#pragma region [WidgetDelegateSubsystemHelper]
void ACoopPuzzleGameState::MULTI_OnShowGlobalNotification_Implementation( const FText& Message )
{
	if( IsNetMode( NM_DedicatedServer ) == true )
		return;

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == false )
		return;

	pWidgetDelegateSubsystem->OnShowGlobalNotification.Broadcast( Message );
}
#pragma endregion