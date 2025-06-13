// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"
#include "EventTriggerObjectBase.generated.h"

class ACoopPuzzleCharacter;
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
	FORCEINLINE int64 GetEventTriggerUID_DE() const { return m_iEventTriggerUID_DE; }

	// 유효성은 BeginPlay의 checkf로 보장되므로 이후에는 null 체크 없이 바로 사용해도 안전합니다.
	FORCEINLINE const FEventTriggerDataRow* GetTriggerData() const { return m_pEventTriggerData; }

	UFUNCTION( BlueprintCallable )
	FORCEINLINE EEventTriggerState GetTriggerState() const { return R_eTriggerState; }

protected:
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_TriggerState();
	void OnRep_TriggerState_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Setting)
	FName EventTriggerID = "";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* DefaultMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* EnableIndicator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* TriggerVolume_DE = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* HintTextVolume_CL = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* HintTextWidget = nullptr;

private:
	UFUNCTION()
	void OnTriggerVolumeBeginOverlap_DE( class UPrimitiveComponent* pOverlappedComp, class AActor* pOtherActor, class UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	UFUNCTION()
	void OnTriggerVolumeEndOverlap_DE( class UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex );

	UFUNCTION()
	void OnIndicatorTextVolumeBeginOverlap_CL( class UPrimitiveComponent* pOverlappedComp, class AActor* pOtherActor, class UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	UFUNCTION()
	void OnIndicatorTextVolumeEndOverlap_CL( class UPrimitiveComponent* pOverlappedComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 iOtherBodyIndex );

	UFUNCTION()
	void OnTriggered_DE( EEventTriggerResult eResult );

	UFUNCTION()
	void SetTriggerState_DE( EEventTriggerState eTriggerState );

	void UpdateHintTextWidget();

	bool m_bIHintTextVolumeOverlapped_CL = false;

	// 반드시 SetTriggerState_DE()를 통해서만 R_eTriggerState를 변경할 것!
	UPROPERTY( ReplicatedUsing=OnRep_TriggerState )
	EEventTriggerState R_eTriggerState = EEventTriggerState::Disabled;

	int64 m_iEventTriggerUID_DE = INVALID_TRIGGER_UID;

	// 이벤트 트리거 데이터 테이블(FEventTriggerDataRow) 고정 포인터 입니다.
	// BeginPlay에서 한 번만 세팅되며, 이후 변경/해제되지 않습니다.
	// 유효성은 BeginPlay의 checkf로 보장되므로 이후에는 null 체크 없이 바로 사용해도 안전합니다.
	const FEventTriggerDataRow* m_pEventTriggerData = nullptr;
};
