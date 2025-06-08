// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CoopPuzzlePlayerState.generated.h"

class ACoopPuzzleCharacter;
struct FItemSyncInfo;

/**
 * WidgetDelegateSubsystem의 DE→CL 네트워크 허브 역할을 담당합니다.
 * DE에서 모든 PlayerState가 공통 Delegate에 바인딩되고, Delegate 브로드캐스트 시 UID를 확인하여 해당 CL에만 이벤트를 전달합니다.
 * 소규모/포폴에 최적화된 구조이며, 대규모 환경에선 UID→Delegate Map 방식 등으로 리팩토링을 고려할 수 있습니다.
 */
UCLASS()
class COOPPUZZLE_API ACoopPuzzlePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void OnPossessed( int64 iPlayerUID );
	void OnUnpossessed( int64 iPlayerUID );

private:
#pragma region [WidgetDelegateSubsystemHelper]
	UFUNCTION( Client, Reliable )
	void CLIENT_OnShowLocalNotification( const FText& Message );
	void CLIENT_OnShowLocalNotification_Implementation( const FText& Message );
#pragma endregion

#pragma region [ItemSubsystemHelper]
	UFUNCTION( Client, Reliable )
	void CLIENT_OnUpdateInventoryItem( const TArray<FItemSyncInfo>& arrUpdateItemInfos );
	void CLIENT_OnUpdateInventoryItem_Implementation( const TArray<FItemSyncInfo>& arrUpdateItemInfos );
#pragma endregion
};
