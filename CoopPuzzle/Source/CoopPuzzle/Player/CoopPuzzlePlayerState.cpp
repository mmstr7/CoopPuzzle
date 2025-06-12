// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Player/CoopPuzzlePlayerState.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Data/CoopPuzzleStructs.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "CoopPuzzle/Subsystem/LevelSequenceSubsystem.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"

void ACoopPuzzlePlayerState::BindEventDelegates_DE( int64 iPlayerUID )
{
	checkf( IsNetMode( NM_DedicatedServer ) == true, TEXT( "DE Only." ) );

	if( IsValid( GetGameInstance() ) == false )
		return;

	// WidgetDelegateSubsystem 헬퍼 함수 바인딩
	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>();
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnShowLocalNotification_ToClient.FindOrAdd( iPlayerUID ).AddUObject( this, &ACoopPuzzlePlayerState::CLIENT_OnShowLocalNotification );
		pWidgetDelegateSubsystem->OnPlayerInventoryUpdated_ToClient.FindOrAdd( iPlayerUID ).AddUObject( this, &ACoopPuzzlePlayerState::CLIENT_OnPlayerInventoryUpdated );
	}

	// ItemSubsystem 헬퍼 함수 바인딩
	UItemSubsystem* pItemSubsystem = GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if( IsValid( pItemSubsystem ) == true )
	{
		pItemSubsystem->OnUpdateInventoryItem_ToClient.FindOrAdd( iPlayerUID ).BindUObject( this, &ACoopPuzzlePlayerState::CLIENT_OnUpdateInventoryItem );
	}

	// LevelSequenceSubsystem 헬퍼 함수 바인딩
	ULevelSequenceSubsystem* pLevelSequenceSubsystem = GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>();
	if( IsValid( pLevelSequenceSubsystem ) == true )
	{
		pLevelSequenceSubsystem->OnPlayLevelSequence_ToClient.FindOrAdd( iPlayerUID ).BindUObject( this, &ACoopPuzzlePlayerState::CLIENT_OnPlayLevelSequence );
		pLevelSequenceSubsystem->OnStopLevelSequence_ToClient.FindOrAdd( iPlayerUID ).BindUObject( this, &ACoopPuzzlePlayerState::CLIENT_OnStopLevelSequence );
	}

	CLIENT_BindEventDelegates( iPlayerUID );
}

void ACoopPuzzlePlayerState::UnbindEventDelegates_DE( int64 iPlayerUID )
{
	checkf( IsNetMode( NM_DedicatedServer ) == true, TEXT( "DE Only." ) );

	if( IsValid( GetGameInstance() ) == false )
		return;

	// WidgetDelegateSubsystem 헬퍼 함수 언바인딩
	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>();
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnShowLocalNotification_ToClient.Remove( iPlayerUID );
		pWidgetDelegateSubsystem->OnPlayerInventoryUpdated_ToClient.Remove( iPlayerUID );
	}

	// ItemSubsystem 헬퍼 함수 언바인딩
	UItemSubsystem* pItemSubsystem = GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if( IsValid( pItemSubsystem ) == true )
	{
		pItemSubsystem->OnUpdateInventoryItem_ToClient.Remove( iPlayerUID );
	}

	// LevelSequenceSubsystem 헬퍼 함수 언바인딩
	ULevelSequenceSubsystem* pLevelSequenceSubsystem = GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>();
	if( IsValid( pLevelSequenceSubsystem ) == true )
	{
		pLevelSequenceSubsystem->OnPlayLevelSequence_ToClient.Remove( iPlayerUID );
		pLevelSequenceSubsystem->OnStopLevelSequence_ToClient.Remove( iPlayerUID );
	}

	CLIENT_UnbindEventDelegates( iPlayerUID );
}

void ACoopPuzzlePlayerState::CLIENT_BindEventDelegates_Implementation( int64 PlayerUID )
{
	// LevelSequenceSubsystem 헬퍼 함수 바인딩
	ULevelSequenceSubsystem* pLevelSequenceSubsystem = GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>();
	if( IsValid( pLevelSequenceSubsystem ) == true )
	{
		pLevelSequenceSubsystem->OnLevelSequenceFinished_ToServer.BindUObject( this, &ACoopPuzzlePlayerState::SERVER_OnLevelSequenceFinished );
	}
}

void ACoopPuzzlePlayerState::CLIENT_UnbindEventDelegates_Implementation( int64 iPlayerUID )
{
	// LevelSequenceSubsystem 헬퍼 함수 언바인딩
	ULevelSequenceSubsystem* pLevelSequenceSubsystem = GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>();
	if( IsValid( pLevelSequenceSubsystem ) == true )
	{
		pLevelSequenceSubsystem->OnLevelSequenceFinished_ToServer.Unbind();
	}
}

#pragma region [WidgetDelegateSubsystemHelper]
void ACoopPuzzlePlayerState::CLIENT_OnShowLocalNotification_Implementation( const FText& Message )
{
	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == false )
		return;

	pWidgetDelegateSubsystem->OnShowLocalNotification_ToClient.FindOrAdd( 0 ).Broadcast( Message );
}

void ACoopPuzzlePlayerState::CLIENT_OnPlayerInventoryUpdated_Implementation()
{
	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == false )
		return;

	pWidgetDelegateSubsystem->OnPlayerInventoryUpdated_ToClient.FindOrAdd( 0 ).Broadcast();
}
#pragma endregion

#pragma region [ItemSubsystemHelper]
void ACoopPuzzlePlayerState::CLIENT_OnUpdateInventoryItem_Implementation( const TArray<FItemSyncInfo>& arrUpdateItemInfos )
{
	UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
	if( IsValid( pItemSubsystem ) == false )
		return;

	pItemSubsystem->OnUpdateInventoryItem_ToClient.FindOrAdd( 0 ).ExecuteIfBound( arrUpdateItemInfos );
}
#pragma endregion

#pragma region [LevelSequenceSubsystemHelper]
void ACoopPuzzlePlayerState::CLIENT_OnPlayLevelSequence_Implementation( const FName& LevelSequenceID, int64 iLevelSequenceUID )
{
	ULevelSequenceSubsystem* pLevelSequenceSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>() : nullptr;
	if( IsValid( pLevelSequenceSubsystem ) == false )
		return;

	pLevelSequenceSubsystem->OnPlayLevelSequence_ToClient.FindOrAdd( 0 ).ExecuteIfBound( LevelSequenceID, iLevelSequenceUID );
}

void ACoopPuzzlePlayerState::CLIENT_OnStopLevelSequence_Implementation( int64 iLevelSequenceUID )
{
	ULevelSequenceSubsystem* pLevelSequenceSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>() : nullptr;
	if( IsValid( pLevelSequenceSubsystem ) == false )
		return;

	pLevelSequenceSubsystem->OnStopLevelSequence_ToClient.FindOrAdd( 0 ).ExecuteIfBound( iLevelSequenceUID );
}

void ACoopPuzzlePlayerState::SERVER_OnLevelSequenceFinished_Implementation( int64 iLevelSequenceUID, int64 iPlayerUID )
{
	ULevelSequenceSubsystem* pLevelSequenceSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>() : nullptr;
	if( IsValid( pLevelSequenceSubsystem ) == false )
		return;

	pLevelSequenceSubsystem->OnLevelSequenceFinished_ToServer.ExecuteIfBound( iLevelSequenceUID, iPlayerUID );
}
#pragma endregion