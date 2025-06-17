// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "ItemSubsystem.generated.h"

DECLARE_DELEGATE_OneParam( FUpdateItemDelegate, const TArray<FItemSyncInfo>& );
DECLARE_DELEGATE_FourParams( FTransferItemDelegate, int64/*TargetPlayerUID*/, int64/*ItemUID*/, int32/*ItemCount*/, int64/*SenderPlayerUID*/);

struct FItemSyncInfo;
struct FItemDataRow;

/**
 * CL/DE 공용 서브시스템입니다.
 * 게임 진행 중 아이템을 발급하고, 아이템 UID 및 유저의 아이템 소지 정보를 캐싱합니다.
 * 실무에서는 아이템 UID를 게임 서버에서 발급하지만, 본 포트폴리오에서는 임의로 DE에서 발급합니다.
 * PlayerState를 네트워크 허브로 사용하여 DE와 CL 간 정보를 동기화합니다.
 * CL에서는 간편한 접근을 위해 로컬 플레이어의 PlayerUID는 0으로 일괄 통일합니다.
 */
UCLASS()
class COOPPUZZLE_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 요구 수량은 모두 양수여야 하며, 로컬 유저가 지정된 유저에게 아이템 전달을 시도.
	void TryGiveItemToPlayer_CL( int64 iTargetPlayerUID, int64 iItemUID, int32 iItemCount );

	// 아이템 추가/삭제 공용. 변경 불가 시 내부 데이터 수정 없이 false 반환.
	bool AddItems_DE( int64 iPlayerUID, const TMap<FName/*ItemID*/, int32/*ItemCount*/>& mapItemInfos );

	// 요구 수량은 모두 양수여야 하며, 하나라도 부족하면 false 반환.
	bool HasItems( int64 iPlayerUID/*CL은 0으로 통일*/, const TMap<FName/*ItemID*/, int32/*ItemCount*/>& mapItemInfos) const;

	void GetItemID( int64 iItemUID, FName& ItemID ) const;
	int32 GetItemCount( int64 iItemUID ) const;
	UTexture2D* GetItemIcon( int64 iItemUID ) const;
	void GetPlayerInventoryItemUIDs( int64 iPlayerUID/*CL은 0으로 통일*/, TArray<int64>& arrItemUIDs, bool bSort ) const;

	// [주의] 외부에서 직접 호출하지 마세요. PlayerState를 경유하여 네트워크 통신(DE → CL)에 사용되는 델리게이트입니다.
	TMap<int64/*PlayerUID*/, FUpdateItemDelegate> OnUpdateInventoryItem_ToClient;

	// [주의] 외부에서 직접 호출하지 마세요. PlayerState를 경유하여 네트워크 통신(CL → DE)에 사용되는 델리게이트입니다. (TargetUID, ItemUID, ItemCount, SenderUID)
	FTransferItemDelegate OnTransferItemBetweenPlayers_ToServer;

protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

private:
	UFUNCTION()
	void UpdateItems( int64 iPlayerUID, const TArray<FItemSyncInfo>& arrUpdateItemInfos );

	UFUNCTION()
	void TransferItemBetweenPlayers( int64 iTargetPlayerUID, int64 iItemUID, int32 iItemCount, int64 iSenderPlayerUID );

	void LoadItemDataIfNotLoaded();

	// 이하 인벤토리/아이템 관련 캐싱 멤버변수들입니다.
	// [중요] UpdateItems()에서만 변경, 외부 직접 수정 금지! (외부에서 변경 시 데이터 불일치, 버그 위험이 있습니다.)
	TMap<int64/*PlayerUID*/, TSet<int64/*ItemUID*/>> m_mapPlayerInventoryItems;
	TMap<int64/*ItemUID*/, int64/*PlayerUID*/> m_mapCachedInventoryItemOwners;
	TMap<FName/*ItemID*/, TSet<int64/*ItemUID*/>> m_mapGeneratedItemUIDs;
	TMap<int64/*ItemUID*/, TPair<FName/*ItemID*/, int32/*Count*/>> m_mapGeneratedItemInfos;

	// 최초 아이템 업데이트 시점에 데이터 테이블 전체를 1회만 로드&캐싱합니다. (Lazy Loading)
	TMap<FName, FItemDataRow*> m_mapCachedItemData;
};
