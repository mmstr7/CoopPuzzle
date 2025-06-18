// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "CoopPuzzle/Data/CoopPuzzleStructs.h"
#include "CoopPuzzlePlayerState.generated.h"

class ACoopPuzzleCharacter;
struct FItemSyncInfo;

/**
 * WidgetDelegateSubsystem의 DE→CL 네트워크 허브 역할을 담당합니다.
 * DE에서는 주인 플레이어의 PlayerUID에 해당하는 Delegate에 PlayerState의 Client RPC 헬퍼 함수가 바인딩됩니다.
 * CL에서는 구독하는 클래스에서 이벤트 바인딩 시 PlayerUID의 동기화 여부를 보장할 수 없으므로, CL 측에서는 PlayerUID를 0으로 통일하여 사용합니다.
 */
UCLASS()
class COOPPUZZLE_API ACoopPuzzlePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	// Client RPC 함수 바인딩
	void BindEventDelegates_DE( int64 iPlayerUID );
	void UnbindEventDelegates_DE( int64 iPlayerUID );

private:
	// Server RPC 함수 바인딩
	UFUNCTION(Client, Reliable)
	void CLIENT_BindEventDelegates( int64 iPlayerUID );
	void CLIENT_BindEventDelegates_Implementation( int64 iPlayerUID );

	UFUNCTION(Client, Reliable)
	void CLIENT_UnbindEventDelegates( int64 iPlayerUID );
	void CLIENT_UnbindEventDelegates_Implementation( int64 iPlayerUID );

#pragma region [WidgetDelegateSubsystemHelper]
	UFUNCTION( Client, Reliable )
	void CLIENT_OnShowLocalNotification( const FText& Message );
	void CLIENT_OnShowLocalNotification_Implementation( const FText& Message );

	UFUNCTION( Client, Reliable )
	void CLIENT_OnPlayerInventoryUpdated();
	void CLIENT_OnPlayerInventoryUpdated_Implementation();

	UFUNCTION( Client, Reliable )
	void CLIENT_OnShowItemNotifications( const TArray<FItemNotifyInfo>& arrNotificationInfos );
	void CLIENT_OnShowItemNotifications_Implementation( const TArray<FItemNotifyInfo>& arrNotificationInfos );

	UFUNCTION( Client, Reliable )
	void CLIENT_OnShowPlayerInputIcon( EPlayerInputType ePlayerInputType, bool bShow );
	void CLIENT_OnShowPlayerInputIcon_Implementation( EPlayerInputType ePlayerInputType, bool bShow );
#pragma endregion

#pragma region [ItemSubsystemHelper]
	UFUNCTION( Client, Reliable )
	void CLIENT_OnUpdateInventoryItem( const TArray<FItemSyncInfo>& arrUpdateItemInfos );
	void CLIENT_OnUpdateInventoryItem_Implementation( const TArray<FItemSyncInfo>& arrUpdateItemInfos );

	UFUNCTION( Server, Reliable )
	void SERVER_OnTransferItemBetweenPlayers( int64 iTargetPlayerUID, int64 iItemUID, int32 iItemCount, int64 iSenderPlayerUID );
	void SERVER_OnTransferItemBetweenPlayers_Implementation( int64 iTargetPlayerUID, int64 iItemUID, int32 iItemCount, int64 iSenderPlayerUID );
#pragma endregion
	
#pragma region [LevelSequenceSubsystemHelper]
	UFUNCTION( Client, Reliable )
	void CLIENT_OnPlayLevelSequence( const FName& LevelSequenceID, int64 iLevelSequenceUID );
	void CLIENT_OnPlayLevelSequence_Implementation( const FName& LevelSequenceID, int64 iLevelSequenceUID );

	UFUNCTION( Client, Reliable )
	void CLIENT_OnStopLevelSequence( int64 iLevelSequenceUID );
	void CLIENT_OnStopLevelSequence_Implementation( int64 iLevelSequenceUID );

	UFUNCTION( Server, Reliable )
	void SERVER_OnLevelSequenceFinished( int64 iLevelSequenceUID, int64 iPlayerUID );
	void SERVER_OnLevelSequenceFinished_Implementation( int64 iLevelSequenceUID, int64 iPlayerUID );
#pragma endregion
};
