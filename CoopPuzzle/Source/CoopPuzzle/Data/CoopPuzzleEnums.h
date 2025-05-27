// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
};

UENUM(BlueprintType)
enum class EEventTriggerEffect : uint8
{
	None				UMETA( Hidden ),
	PickUpAttach		UMETA( DisplayName = "PickUpAttach" ),
};

UENUM(BlueprintType)
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
	Event				UMETA( DisplayName = "EventDataTable" ),
	Item				UMETA( DisplayName = "ItemDataTable" ),
	MAX					UMETA( Hidden ),
};