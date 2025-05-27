// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzleData.generated.h"

USTRUCT(BlueprintType)
struct FEventDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Comment = "";

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "이벤트 실행 모드" ) )
    EEventTriggerMode EventTriggerMode = EEventTriggerMode::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "이벤트 실행 조건" ) )
    EEventTriggerCondition EventTriggerCondition = EEventTriggerCondition::None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = ( ToolTip = "이벤트 실행 효과" ) )
    EEventTriggerEffect EventTriggerEffect = EEventTriggerEffect::None;
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
