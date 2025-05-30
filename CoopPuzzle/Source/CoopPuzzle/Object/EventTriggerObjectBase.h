// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "EventTriggerObjectBase.generated.h"

struct FEventTriggerDataRow;

/**
 * 트리거 볼륨에 진입한 플레이어가 이벤트를 실행할 수 있는 트리거 액터입니다.
 * EventTriggerID가 EventTriggerDataTable의 키와 일치해야 사용 가능합니다.
 * 실제 이벤트 실행은 UEventTriggerManagerSubsystem이 처리합니다.
 */
UCLASS(BlueprintType)
class COOPPUZZLE_API AEventTriggerObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEventTriggerObjectBase();

	FORCEINLINE const FName& GetEventTriggerID() const { return EventTriggerID; }
	//FORCEINLINE const FEventTriggerDataRow* GetEventTriggerData() const { return m_pEventTriggerData; }

protected:
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

	UFUNCTION( BlueprintNativeEvent )
	void OnRep_TriggerState();
	virtual void OnRep_TriggerState_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setting)
	FName EventTriggerID = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* DefaultMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* TriggerVolume = nullptr;

private:
	UFUNCTION()
	void OnTriggerVolumeBeginOverlap_DE( class UPrimitiveComponent* pOverlappedComp, class AActor* pOtherActor, class UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	UFUNCTION()
	void OnTriggerVolumeEndOverlap_DE( class UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex );

	UFUNCTION()
	void OnTriggered_DE( EEventTriggerResult eResult );

	UPROPERTY( ReplicatedUsing = OnRep_TriggerState )
	EEventTriggerState R_eTriggerState = EEventTriggerState::Enabled;

	const FEventTriggerDataRow* m_pEventTriggerData = nullptr;
};
