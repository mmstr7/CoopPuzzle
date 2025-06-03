// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CoopPuzzlePlayerState.generated.h"

class ACoopPuzzleCharacter;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API ACoopPuzzlePlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
private:
#pragma region [WidgetDelegateSubsystemHelper]
	UFUNCTION()
	void OnShowLocalNotification_DE( const int64& iPlayerUID, const FText& Message );

	UFUNCTION( Client, Reliable )
	void CLIENT_OnShowLocalNotification( const int64& iPlayerUID, const FText& Message );
	void CLIENT_OnShowLocalNotification_Implementation( const int64& iPlayerUID, const FText& Message );
#pragma endregion
};
