// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/PlayerManagerSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Object/EventTriggerObjectBase.h"

void UPlayerManagerSubsystem::RegisterPlayer( ACoopPuzzleCharacter* pPlayer )
{
	if( IsValid( pPlayer ) == false )
		return;

	mapCachedPlayer.Add( pPlayer->GetPlayerUID(), pPlayer );
}

void UPlayerManagerSubsystem::UnregisterPlayer( const int64& iPlayerUID )
{
	mapCachedPlayer.Remove( iPlayerUID );
}

bool UPlayerManagerSubsystem::IsPlayersInRange( int64 iPlayerAUID, int64 iPlayerBUID, float fMaxDistance )
{
	ACoopPuzzleCharacter* pPlayerA = mapCachedPlayer.FindRef( iPlayerAUID ).Get();
	if( IsValid( pPlayerA ) == false )
		return false;

	ACoopPuzzleCharacter* pPlayerB = mapCachedPlayer.FindRef( iPlayerBUID ).Get();
	if( IsValid( pPlayerB ) == false )
		return false;

	return FVector::DistSquared( pPlayerA->GetActorLocation(), pPlayerB->GetActorLocation() ) <= FMath::Square( fMaxDistance );
}

bool UPlayerManagerSubsystem::ShouldCreateSubsystem( UObject* Outer ) const
{
	const UGameInstance* pGameInstance = Cast<UGameInstance>( Outer );
	return IsValid( pGameInstance ) == true && pGameInstance->IsDedicatedServerInstance() == true;
}
