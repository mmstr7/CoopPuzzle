// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoopPuzzleMainHUD.generated.h"

class UNotificationText;

class UTextBlock;
/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UCoopPuzzleMainHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UNotificationText* GlobalNotification;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UNotificationText* LocalNotification;

private:
	UFUNCTION()
	void ShowGlobalNotification( const FText& Message );

	UFUNCTION()
	void ShowLocalNotification( const FText& Message );
};
