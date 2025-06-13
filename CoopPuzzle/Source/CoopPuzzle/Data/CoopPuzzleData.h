// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzle/Data/CoopPuzzleStructs.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "CoopPuzzleData.generated.h"

USTRUCT(BlueprintType)
struct FEventTriggerDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "트리거 이름" ) )
    FText TriggetName = FText();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "개발자용 주석" ) )
    FString Comment = "";

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "기본 상태" ) )
	EEventTriggerState DefaultState = EEventTriggerState::Enabled;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "이벤트 실행 조건 충족 시 자동 실행 여부" ) )
    bool AutoTriggerOnCondition = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "수동 실행 모드. 반드시 유저와 트리거가 연결된 상태에서 실행 가능.", EditCondition = "AutoTriggerOnCondition == false" ) )
    EManualTriggerMode ManualTriggerMode = EManualTriggerMode::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "조건 없이 실행 가능", EditCondition = "AutoTriggerOnCondition == false" ) )
    bool Unconditional = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "이벤트 실행 조건", EditCondition = "Unconditional == false" ) )
    FEventTriggerConditionParams EventTriggerCondition;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "이벤트 실행 효과" ) )
    FEventTriggerSuccessEffectParams EventTriggerSuccessEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "힌트 범위에 들어왔을 때 출력할 텍스트" ) )
    FText HintText = FText();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "결과에 따른 전체 알림 텍스트" ) )
    TMap<EEventTriggerResult, FText> GlobalNotifications;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "결과에 따른 로컬 알림 텍스트" ) )
    TMap<EEventTriggerResult, FText> LocalNotifications;
};

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ItemName = FText();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Comment = "";

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* ItemIcon = nullptr;
};

USTRUCT( BlueprintType )
struct FLevelSequenceDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	FString Comment = "";

	UPROPERTY( EditAnywhere, BlueprintReadOnly )
	ULevelSequence* LevelSequence = nullptr;
};