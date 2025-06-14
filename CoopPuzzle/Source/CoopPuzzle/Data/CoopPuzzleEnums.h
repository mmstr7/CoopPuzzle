// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEventTriggerState : uint8
{
	Disabled				UMETA( DisplayName = "Disabled" ),
	Enabled					UMETA( DisplayName = "Enabled" ),
	Triggered				UMETA( DisplayName = "Triggered" ),
};

UENUM(BlueprintType)
enum class EManualTriggerMode : uint8
{
	None							UMETA( Hidden ),
	InputInteractKey				UMETA( DisplayName = "InputInteractKey" ),
	InTriggerVolume_Once			UMETA( DisplayName = "InTriggerVolume_Once" ),
	InTriggerVolume_Stay			UMETA( DisplayName = "InTriggerVolume_Stay" ),
	InTriggerVolume_Toggle			UMETA( DisplayName = "InTriggerVolume_Toggle" ),
	InTriggerVolume_ConditionStay	UMETA( DisplayName = "InTriggerVolume_ConditionStay" ),
};

UENUM(BlueprintType)
enum class EEventTriggerCondition : uint8
{
	None					UMETA( Hidden ),
	HasItem_Consume			UMETA( DisplayName = "HasItem_Consume" ),
	HasItem_Possession		UMETA( DisplayName = "HasItem_Possession" ),
	OtherTrigger_Triggered	UMETA( DisplayName = "OtherTrigger_Triggered" ),
};

UENUM(BlueprintType)
enum class EEventTriggerSuccessEffect : uint8
{
	None					UMETA( Hidden ),
	GainItem				UMETA( DisplayName = "GainItem" ),
	PlayLevelSequence		UMETA( DisplayName = "PlayLevelSequence" ),
};

UENUM( BlueprintType )
enum class EEventTriggerResult : uint8
{
	None					UMETA( Hidden ),
	Success					UMETA( DisplayName = "Success" ),
	Failed 					UMETA( DisplayName = "Failed" ),
};

UENUM(BlueprintType)
enum class EPlayerInputType : uint8
{
	None					UMETA( Hidden ),
	Interact				UMETA( DisplayName = "Interact" ),
};

UENUM(BlueprintType)
enum class EDataTableType : uint8
{
	EventTrigger			UMETA( DisplayName = "EventTriggerDataTable" ),
	Item					UMETA( DisplayName = "ItemDataTable" ),
	LevelSequence			UMETA( DisplayName = "LevelSequence" ),
	MAX						UMETA( Hidden ),
};

UENUM(BlueprintType)
enum class EProcessState : uint8
{
	None					UMETA( Hidden ),
	Playing					UMETA( DisplayName = "Playing" ),
	Waiting					UMETA( DisplayName = "Waiting" ),
	Finished				UMETA( DisplayName = "Finished" ),
};

UENUM(BlueprintType)
enum class ETooltipType : uint8
{
	Item					UMETA( DisplayName = "Item" ),
	MAX						UMETA( Hidden ),
};