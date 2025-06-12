// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemSubsystem.generated.h"

DECLARE_DELEGATE_OneParam( FUpdateItemDelegate, const TArray<FItemSyncInfo>& );

struct FItemSyncInfo;
struct FItemDataRow;

/**
 * CL/DE 공용 서브시스템입니다.
 * 게임 진행 중 아이템을 발급하고, 아이템 UID 및 유저의 아이템 소지 정보를 캐싱합니다.
 * 실무에서는 아이템 UID를 게임 서버에서 발급하지만, 본 포트폴리오에서는 임의로 DE에서 발급합니다.
 * PlayerState를 네트워크 허브로 사용하여 DE와 CL 간 정보를 동기화합니다.
 * 본 포트폴리오에서는 아이템을 직접 사용하는 기능이 없으므로, CL에서는 자신의 아이템 정보만 조회할 수 있으며 데이터 수정 요청은 할 수 없습니다.
 * CL에서는 간편한 접근을 위해 로컬 플레이어의 PlayerUID는 0으로 일괄 통일합니다.
 */
UCLASS()
class COOPPUZZLE_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	int32 GetItemCount( int64 iItemUID ) const;
	UTexture2D* GetItemIcon( int64 iItemUID ) const;

	// CL 에서는 PlayerUID를 0으로 통일
	void GetPlayerInventoryItemUIDs( int64 iPlayerUID, TArray<int64>& arrItemUIDs, bool bSort );

	bool HasItems( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos ) const;

	// 아이템 추가/삭제 공용. 변경 불가 시 내부 데이터 수정 없이 false 반환.
	bool AddItems_DE( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos );

	// 아이템 정보 동기화용 델리게이트 입니다. 위젯 업데이트는 반드시 WidgetDelegateSubsystem을 통해 처리하세요.
	TMap<int64/*PlayerUID*/, FUpdateItemDelegate> OnUpdateInventoryItem_ToClient;

protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

private:
	UFUNCTION()
	void UpdateItems( int64 iPlayerUID, const TArray<FItemSyncInfo>& arrUpdateItemInfos );

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
