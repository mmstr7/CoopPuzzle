// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerManagerSubsystem.generated.h"

class ACoopPuzzleCharacter;
class AEventTriggerObjectBase;

/**
 * DE Àü¿ë
 */
UCLASS()
class COOPPUZZLE_API UPlayerManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void RegisterPlayer( ACoopPuzzleCharacter* pPlayer );
	void UnregisterPlayer( const int64& iPlayerUID );
	FORCEINLINE const TWeakObjectPtr<ACoopPuzzleCharacter> GetPlayer( const int64& iPlayerUID ) { return mapCachedPlayer.FindRef( iPlayerUID ); }

	bool IsPlayersInRange( int64 iPlayerAUID, int64 iPlayerBUID, float fMaxDistance );

protected:
	virtual bool ShouldCreateSubsystem( UObject* Outer ) const override;

private:
	UPROPERTY()
	TMap<int64, TWeakObjectPtr<ACoopPuzzleCharacter>> mapCachedPlayer;
};
