// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "TooltipSubsystem.generated.h"

class UTooltipBase;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UTooltipSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UTooltipBase* GetTooltipWidget( ETooltipType eTooltipType, const FName& Key );
	
protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

private:
	UPROPERTY()
	TMap<ETooltipType, UTooltipBase*> m_mapCachedTooltip;

	// 각 툴팁에 해당하는 툴팁 위젯 클래스
	TMap<ETooltipType, TSubclassOf<UTooltipBase>> m_mapTooltipClass;
};
