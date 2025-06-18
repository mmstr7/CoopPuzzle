// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"
#include "CoopPuzzleStructs.generated.h"

class ULevelSequencePlayer;

USTRUCT( BlueprintType )
struct FEventTriggerConditionParams
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EEventTriggerCondition ConditionType = EEventTriggerCondition::None;

	// Condition(����) ������ ���� �Ķ���� ���� �ʿ��� ����, �ʿ� ���� ���� �ֽ��ϴ�.
	// ��� ����� EEventTriggerCondition�� �� �׸� �ּ��� �����ϼ���.
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TMap<FName, int32> Params;
};

USTRUCT( BlueprintType )
struct FEventTriggerSuccessEffectParams
{
	GENERATED_BODY()

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EEventTriggerSuccessEffect EffectType = EEventTriggerSuccessEffect::None;

	// Effect(ȿ��) ������ ���� �Ķ���� ���� �ʿ��� ����, �ʿ� ���� ���� �ֽ��ϴ�.
	// ��� ����� EEventTriggerEffect�� �� �׸� �ּ��� �����ϼ���.
	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TMap<FName, int32> Params;
};

USTRUCT( BlueprintType )
struct FItemSyncInfo
{
	GENERATED_BODY()

	FItemSyncInfo() {}

	FItemSyncInfo( int64 ItemUID_, const FName& ItemID_, int32 Count_, EItemNotification NotificationType_ )
		: ItemUID(ItemUID_ ), ItemID( ItemID_ ), Count(Count_), NotificationType( NotificationType_ ) { }

	UPROPERTY( BlueprintReadOnly )
	int64 ItemUID = -1;

	UPROPERTY( BlueprintReadOnly )
	FName ItemID;

    UPROPERTY( BlueprintReadOnly )
	int32 Count = 0;

	// ���� �˸� ����
	UPROPERTY( BlueprintReadOnly )
	EItemNotification NotificationType = EItemNotification::None;
};

USTRUCT( BlueprintType )
struct FItemNotifyInfo
{
	GENERATED_BODY()

public:
	FItemNotifyInfo(){}

	FItemNotifyInfo( const FName& ItemID_, EItemNotification ItemNotificationType_ )
		: ItemID( ItemID_ ), ItemNotificationType(ItemNotificationType_) { }

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	FName ItemID;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	EItemNotification ItemNotificationType = EItemNotification::None;
};