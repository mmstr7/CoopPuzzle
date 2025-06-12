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
		if( IsValid( pWidgetDelegateSubsystem ) == true && pWidgetDelegateSubsystem->OnShowGlobalNotification_ToClient.IsBoundToObject( this ) == false )
		{
			pWidgetDelegateSubsystem->OnShowGlobalNotification_ToClient.AddUObject( this, &ACoopPuzzleGameState::MULTI_OnShowGlobalNotification );
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

void ACoopPuzzleGameState::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );

	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		// WidgetDelegateSubsystem 헬퍼 함수 언바인딩
		UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
		if( IsValid( pWidgetDelegateSubsystem ) == true )
		{
			pWidgetDelegateSubsystem->OnShowGlobalNotification_ToClient.RemoveAll( this );
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

	pWidgetDelegateSubsystem->OnShowGlobalNotification_ToClient.Broadcast( Message );
}
#pragma endregion