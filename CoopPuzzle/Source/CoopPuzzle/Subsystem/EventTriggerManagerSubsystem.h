// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EventTriggerManagerSubsystem.generated.h"

class ACoopPuzzleCharacter;

DECLARE_LOG_CATEGORY_EXTERN( LogEventTriggerManagerSubsystem, Log, All );

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UEventTriggerManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void TriggerEvent( const FName& TriggerID, const ACoopPuzzleCharacter* pInstigator ) const;

protected:
	virtual bool ShouldCreateSubsystem( UObject* Outer ) const override;
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;
	virtual void Deinitialize() override;
};
