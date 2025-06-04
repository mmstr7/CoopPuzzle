// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzleData.generated.h"

USTRUCT(BlueprintType)
struct FEventTriggerDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Comment = "";

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "기본 상태" ) )
    EEventTriggerState DefaultState = EEventTriggerState::Enabled;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "이벤트 실행 모드" ) )
    EEventTriggerMode EventTriggerMode = EEventTriggerMode::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "이벤트 실행 조건" ) )
    EEventTriggerCondition EventTriggerCondition = EEventTriggerCondition::None;

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
};

USTRUCT(BlueprintType)
struct FLevelSequenceDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ItemName = FText();

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Comment = "";
};
