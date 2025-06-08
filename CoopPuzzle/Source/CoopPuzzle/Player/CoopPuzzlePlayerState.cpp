// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Player/CoopPuzzlePlayerState.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Data/CooppuzzleStructs.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"

void ACoopPuzzlePlayerState::OnPossessed( int64 iPlayerUID )
{
	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		// WidgetDelegateSubsystem 헬퍼 함수 바인딩
		UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
		if( IsValid( pWidgetDelegateSubsystem ) == true )
		{
			pWidgetDelegateSubsystem->OnShowLocalNotification.FindOrAdd( iPlayerUID ).AddUObject( this, &ACoopPuzzlePlayerState::CLIENT_OnShowLocalNotification );
		}

		// ItemSubsystem 헬퍼 함수 바인딩
		UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
		if( IsValid( pItemSubsystem ) == true )
		{
			pItemSubsystem->OnUpdateInventoryItem.FindOrAdd( iPlayerUID ).BindUObject( this, &ACoopPuzzlePlayerState::CLIENT_OnUpdateInventoryItem );
		}
	}
}

void ACoopPuzzlePlayerState::OnUnpossessed( int64 iPlayerUID )
{
	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		// WidgetDelegateSubsystem 헬퍼 함수 언바인딩
		UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
		if( IsValid( pWidgetDelegateSubsystem ) == true )
		{
			pWidgetDelegateSubsystem->OnShowLocalNotification.Remove( iPlayerUID );
		}

		// ItemSubsystem 헬퍼 함수 언바인딩
		UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
		if( IsValid( pItemSubsystem ) == true )
		{
			pItemSubsystem->OnUpdateInventoryItem.Remove( iPlayerUID );
		}
	}
}

void ACoopPuzzlePlayerState::CLIENT_OnShowLocalNotification_Implementation( const FText& Message )
{
	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == false )
		return;

	pWidgetDelegateSubsystem->OnShowLocalNotification.FindOrAdd( 0 ).Broadcast( Message );
}

void ACoopPuzzlePlayerState::CLIENT_OnUpdateInventoryItem_Implementation( const TArray<FItemSyncInfo>& arrUpdateItemInfos )
{
	UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
	if( IsValid( pItemSubsystem ) == false )
		return;

	pItemSubsystem->OnUpdateInventoryItem.FindOrAdd( 0 ).ExecuteIfBound( arrUpdateItemInfos );
}
