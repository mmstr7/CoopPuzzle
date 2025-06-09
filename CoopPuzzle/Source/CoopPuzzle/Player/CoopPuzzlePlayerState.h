// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CoopPuzzlePlayerState.generated.h"

class ACoopPuzzleCharacter;
struct FItemSyncInfo;

/**
 * WidgetDelegateSubsystem의 DE→CL 네트워크 허브 역할을 담당합니다.
 * DE에서는 주인 플레이어의 PlayerUID에 해당하는 Delegate에 PlayerState의 Client RPC 헬퍼 함수가 바인딩됩니다.
 * CL에서 동일 델리게이트 호출 시에는 PlayerUID를 0으로 통일하여 사용합니다. (접근 용이)
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

	UFUNCTION( Client, Reliable )
	void CLIENT_OnPlayerInventoryUpdated();
	void CLIENT_OnPlayerInventoryUpdated_Implementation();
#pragma endregion

#pragma region [ItemSubsystemHelper]
	UFUNCTION( Client, Reliable )
	void CLIENT_OnUpdateInventoryItem( const TArray<FItemSyncInfo>& arrUpdateItemInfos );
	void CLIENT_OnUpdateInventoryItem_Implementation( const TArray<FItemSyncInfo>& arrUpdateItemInfos );
#pragma endregion
};
