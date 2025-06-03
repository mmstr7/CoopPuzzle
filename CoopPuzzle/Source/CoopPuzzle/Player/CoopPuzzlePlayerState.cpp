// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Player/CoopPuzzlePlayerState.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"

void ACoopPuzzlePlayerState::BeginPlay()
{
	Super::BeginPlay();

	if( IsNetMode( NM_DedicatedServer ) == true )
	{
		// WidgetDelegateSubsystem 헬퍼 함수 바인딩
		UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
		if( IsValid( pWidgetDelegateSubsystem ) == true )
		{
			pWidgetDelegateSubsystem->OnShowLocalNotification.AddDynamic( this, &ACoopPuzzlePlayerState::OnShowLocalNotification_DE );
		}
	}
}

void ACoopPuzzlePlayerState::OnShowLocalNotification_DE( const int64& iPlayerUID, const FText& Message )
{
	ACoopPuzzleCharacter* pPlayer = static_cast< ACoopPuzzleCharacter* >( GetPlayerController()->GetPawn() );
	if( IsValid( pPlayer ) == false || pPlayer->GetPlayerUID() != iPlayerUID )
		return;

	CLIENT_OnShowLocalNotification( iPlayerUID, Message );
}

void ACoopPuzzlePlayerState::CLIENT_OnShowLocalNotification_Implementation( const int64& iPlayerUID, const FText& Message )
{
	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == false )
		return;

	pWidgetDelegateSubsystem->OnShowLocalNotification.Broadcast( iPlayerUID, Message );
}
