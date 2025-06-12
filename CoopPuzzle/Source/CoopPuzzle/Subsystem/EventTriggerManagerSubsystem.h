// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzle/Data/CoopPuzzleStructs.h"
#include "EventTriggerManagerSubsystem.generated.h"

class ACoopPuzzleCharacter;
class AEventTriggerObjectBase;

DECLARE_DELEGATE_OneParam( FOnEventTriggerCompleted, EEventTriggerResult );
DECLARE_LOG_CATEGORY_EXTERN( LogEventTriggerManagerSubsystem, Log, All );

struct FEventTriggerHandle
{
public:
	FEventTriggerHandle() {}
	FEventTriggerHandle( TWeakObjectPtr<AEventTriggerObjectBase> pTrigger, const FOnEventTriggerCompleted& CompletedDelegate )
		: EventTrigger( pTrigger ), OnCompletedDelegate( CompletedDelegate ) {}

	TWeakObjectPtr<AEventTriggerObjectBase> EventTrigger;
	FOnEventTriggerCompleted OnCompletedDelegate;
};

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
	void TriggerManualEvent( const int64& iPlayerUID, EManualTriggerMode eEventTriggerMode );
	void TriggerAutoEvent( const int64& iPlayerUID, EEventTriggerCondition eConditionType, const FName& ConditionKey );

	void RegisterEventTrigger( AEventTriggerObjectBase* pEventTrigger, const FOnEventTriggerCompleted& OnCompletedCallback );
	void UnregisterEventTrigger( const FName& EventTriggerID );

	void LinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID );
	void UnlinkPlayerToEventTrigger( const int64& iPlayerUID, const FName& EventTriggerID );

protected:
	virtual bool ShouldCreateSubsystem( UObject* Outer ) const override;

private:
	EEventTriggerResult TriggerEvent( const int64& iPlayerUID, const FName& EventTriggerID, bool bIsManualTriggerd, EManualTriggerMode eEventTriggerMode = EManualTriggerMode::None );

	TMap<int64/*PlayerUID*/, FName/*EventTriggerID*/> mapLinkedPlayerToEventTrigger;
	TMap<FName/*EventTriggerID*/, TSet<int64/*PlayerUID*/> > mapLinkedEventTriggerToPlayer;
	TMap<FName/*EventTriggerID*/, FEventTriggerHandle> mapCachedTriggerHandle;

	// 조건 충족 시 즉시 실행해야 하는 트리거 정보. 한 번 트리거 된 데이터는 제거됩니다. (재실행 불가)
	TMap<EEventTriggerCondition, TMap<FName/*EventTriggerID*/, FEventTriggerConditionParams>> mapCachedAutoTriggerConditions;
};
