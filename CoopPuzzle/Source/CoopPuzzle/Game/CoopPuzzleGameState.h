// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CoopPuzzleGameState.generated.h"

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API ACoopPuzzleGameState : public AGameState
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY( EditAnywhere, Category = Setting )
	UUserWidget* MainHUD = nullptr;

private:
#pragma region [WidgetDelegateSubsystemHelper]
	UFUNCTION( NetMulticast, Reliable )
	void MULTI_OnShowGlobalNotification( const FText& Message );
	void MULTI_OnShowGlobalNotification_Implementation( const FText& Message );
#pragma endregion
};
