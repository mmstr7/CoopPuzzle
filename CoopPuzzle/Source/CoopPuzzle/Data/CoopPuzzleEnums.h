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
enum class EEventTriggerMode : uint8
{
	None					UMETA( Hidden ),
	InputInteractKey		UMETA( DisplayName = "InputInteractKey" ),
	InTriggerVolume_Once	UMETA( DisplayName = "InTriggerVolume_Once" ),
	InTriggerVolume_Stay	UMETA( DisplayName = "InTriggerVolume_Stay" ),
};

UENUM(BlueprintType)
enum class EEventTriggerCondition : uint8
{
	None					UMETA( Hidden ),
	Unconditional			UMETA( DisplayName = "Unconditional" ),
	OverlapConditionVolume	UMETA( DisplayName = "OverlapConditionVolume" ),
	HasItem_Consume			UMETA( DisplayName = "HasItem_Consume" ),
	HasItem_Possession		UMETA( DisplayName = "HasItem_Possession" ),
};

UENUM(BlueprintType)
enum class EEventTriggerSuccessEffect : uint8
{
	None					UMETA( Hidden ),
	GainItem				UMETA( DisplayName = "GainItem" ),
};

UENUM( BlueprintType )
enum class EEventTriggerResult : uint8
{
	None					UMETA( Hidden ),
	Success					UMETA( DisplayName = "Success" ),
	Failed 					UMETA( DisplayName = "Failed" ),
	Reset 					UMETA( DisplayName = "Reset" ),
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
	MAX						UMETA( Hidden ),
};
