// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEventTriggerState : uint8
{
	None				UMETA( Hidden ),
	Disabled			UMETA( DisplayName = "Disabled" ),
	Enabled				UMETA( DisplayName = "Enabled" ),
	Triggered			UMETA( DisplayName = "Triggered" ),
};

UENUM(BlueprintType)
enum class EEventTriggerMode : uint8
{
	None				UMETA( Hidden ),
	InputInteractKey	UMETA( DisplayName = "InputInteractKey" ),
	InTriggerVolume		UMETA( DisplayName = "InTriggerVolume" ),
};

UENUM(BlueprintType)
enum class EEventTriggerCondition : uint8
{
	None				UMETA( Hidden ),
	Unconditional		UMETA( DisplayName = "Unconditional" ),
};

UENUM(BlueprintType)
enum class EEventTriggerSuccessEffect : uint8
{
    None                UMETA(Hidden),
    CallCustomEvent     UMETA(DisplayName = "CallCustomEvent"),
};

UENUM(BlueprintType)
enum class EEventTriggerFailureEffect : uint8
{
    None                UMETA(Hidden),
    CallCustomEvent     UMETA(DisplayName = "CallCustomEvent"),
};

UENUM( BlueprintType )
enum class EEventTriggerResult : uint8
{
	None				UMETA( Hidden ),
	Success				UMETA( DisplayName = "Success" ),
	Failed 				UMETA( DisplayName = "Failed" ),
};

UENUM(BlueprintType)
enum class EPlayerInputType : uint8
{
	None				UMETA( Hidden ),
	Interact			UMETA( DisplayName = "Interact" ),
};

UENUM(BlueprintType)
enum class EDataTableType : uint8
{
	EventTrigger		UMETA( DisplayName = "EventTriggerDataTable" ),
	Item				UMETA( DisplayName = "ItemDataTable" ),
	MAX					UMETA( Hidden ),
};

