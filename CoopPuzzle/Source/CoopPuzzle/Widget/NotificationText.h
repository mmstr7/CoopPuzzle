// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Fonts/SlateFontInfo.h"
#include "NotificationText.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UNotificationText : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void AddNotification( const FText& Message );

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Setting )
	FSlateFontInfo NotificationFont;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Setting )
	FSlateColor NotificationColor;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = Setting )
	float NotificationDuration = 3.f;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick( const FGeometry& MyGeometry, float InDeltaTime ) override;

private:
	UPROPERTY( meta = (BindWidget))
	UTextBlock* NotificationText;

	FDateTime LastNotificationTime = FDateTime::MinValue();
	bool bNotificationEnded = true;
};
