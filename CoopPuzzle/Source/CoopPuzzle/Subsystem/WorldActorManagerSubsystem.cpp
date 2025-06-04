// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/WorldActorManagerSubsystem.h"
#include "WorldActorManagerSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Object/EventTriggerObjectBase.h"

void UWorldActorManagerSubsystem::RegisterPlayer( ACoopPuzzleCharacter* pPlayer )
{
	if( IsValid( pPlayer ) == false )
		return;

	mapCachedPlayer.Add( pPlayer->GetPlayerUID(), pPlayer );
}

void UWorldActorManagerSubsystem::UnregisterPlayer( const int64& iPlayerUID )
{
	mapCachedPlayer.Remove( iPlayerUID );
}

void UWorldActorManagerSubsystem::RegisterEventTrigger( AEventTriggerObjectBase* pTrigger )
{
	if( IsValid( pTrigger ) == false )
		return;

	mapEventTrigger.Add( pTrigger->GetEventTriggerID(), pTrigger );
}

void UWorldActorManagerSubsystem::UnregisterEventTrigger( const FName& EventTriggerID )
{
	mapEventTrigger.Remove( EventTriggerID );
}
