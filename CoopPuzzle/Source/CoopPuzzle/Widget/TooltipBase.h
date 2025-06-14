// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipBase.generated.h"

/**
 * 
 */
UCLASS( Abstract )
class COOPPUZZLE_API UTooltipBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetupTooltipWidget( const FName& Key ) PURE_VIRTUAL( UTooltipBase::SetupTooltipWidget );
	
};
