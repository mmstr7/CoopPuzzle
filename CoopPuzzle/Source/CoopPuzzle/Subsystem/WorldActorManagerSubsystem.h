// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldActorManagerSubsystem.generated.h"

class ACoopPuzzleCharacter;
class AEventTriggerObjectBase;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UWorldActorManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void RegisterPlayer( ACoopPuzzleCharacter* pPlayer );
	void UnregisterPlayer( const int64& iPlayerUID );
	FORCEINLINE const TWeakObjectPtr<ACoopPuzzleCharacter> GetPlayer( const int64& iPlayerUID ) { return mapCachedPlayer.FindRef( iPlayerUID ); }

	void RegisterEventTrigger( AEventTriggerObjectBase* pTrigger );
	void UnregisterEventTrigger( const FName& EventTriggerID );
	FORCEINLINE const TWeakObjectPtr<AEventTriggerObjectBase> GetEventTrigger( const FName& TriggerID ) { return mapEventTrigger.FindRef( TriggerID ); }

private:
	TMap<int64, TWeakObjectPtr<ACoopPuzzleCharacter>> mapCachedPlayer;
	TMap<FName, TWeakObjectPtr<AEventTriggerObjectBase>> mapEventTrigger;
};
