// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "EventTriggerManagerSubsystem.generated.h"

class ACoopPuzzleCharacter;
class AEventTriggerObjectBase;

DECLARE_DELEGATE_OneParam( FOnEventTriggerCompleted, EEventTriggerResult );

DECLARE_LOG_CATEGORY_EXTERN( LogEventTriggerManagerSubsystem, Log, All );

/**
 * DE 전용 서브시스템입니다.
 * 게임 내에서 생성된 모든 EventTriggerObject를 관리하며,
 * 플레이어에 링크된 트리거 정보를 저장하고, 해당 트리거의 이벤트를 실행하는 역할을 담당합니다.
 */
UCLASS()
class COOPPUZZLE_API UEventTriggerManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void TriggerEvent( const int64& iPlayerUID, EEventTriggerMode eEventTriggerMode );

	void RegisterEventTrigger( const FName& EventTriggerID, FOnEventTriggerCompleted OnCompletedCallback );
	void UnregisterEventTrigger( const FName& EventTriggerID );

	void LinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID );
	void UnlinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID );

protected:
	virtual bool ShouldCreateSubsystem( UObject* Outer ) const override;

private:
	TMap<int64/*PlayerUID*/, FName/*EventTriggerID*/> mapLinkedPlayerToEventTrigger;
	TMap<FName/*EventTriggerID*/, FOnEventTriggerCompleted> mapTriggerCompletedDelegates;
};
