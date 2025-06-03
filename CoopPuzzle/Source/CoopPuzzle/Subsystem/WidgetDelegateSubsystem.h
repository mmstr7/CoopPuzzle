// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Delegates/DelegateCombinations.h"
#include "WidgetDelegateSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FTextDelegate, const FText&, Text );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FUIDTextDelegate, const int64&, UID, const FText&, Text );

/**
 * DE/CL 공용 서브시스템입니다.
 * 시스템과 위젯 사이의 델리게이트 중개 역할을 담당합니다.
 * 위젯은 관련 이벤트를 바인딩하고, 시스템은 해당 서브시스템을 통해 이벤트를 전달합니다.
 * DE에서 이벤트 호출 시, GameState/PlayerState를 네트워크 허브로 사용해 CL에 알림을 전파합니다.
 */
UCLASS(Blueprintable)
class COOPPUZZLE_API UWidgetDelegateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY( BlueprintAssignable )
	FTextDelegate OnShowGlobalNotification;

	UPROPERTY( BlueprintAssignable )
	FUIDTextDelegate OnShowLocalNotification;

};
