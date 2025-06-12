// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Delegates/DelegateCombinations.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "WidgetDelegateSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam( FTextDelegate, const FText& );
DECLARE_MULTICAST_DELEGATE_OneParam( FProcessStateDelegate, EProcessState );

/**
 * DE/CL 공용 서브시스템입니다.
 * 시스템과 위젯 사이의 델리게이트 중개 역할을 담당합니다.
 * 위젯은 관련 이벤트를 바인딩하고, 시스템은 본 서브시스템을 통해 이벤트를 전달합니다.
 * DE에서 이벤트 호출 시, GameState/PlayerState를 네트워크 허브로 사용해 CL에 알림을 전파합니다.
 * CL에서는 이벤트 바인딩 시 PlayerUID의 동기화 여부를 보장할 수 없으므로, CL 측에서는 PlayerUID를 0으로 통일하여 사용합니다.
 */
UCLASS(Blueprintable)
class COOPPUZZLE_API UWidgetDelegateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FTextDelegate OnShowGlobalNotification_ToClient;

	TMap<int64/*PlayerUID (CL은 0으로 통일)*/, FTextDelegate> OnShowLocalNotification_ToClient;

	TMap<int64/*PlayerUID (CL은 0으로 통일)*/, FSimpleMulticastDelegate> OnPlayerInventoryUpdated_ToClient;

	FProcessStateDelegate OnLevelSequenceStateChanged;
};
