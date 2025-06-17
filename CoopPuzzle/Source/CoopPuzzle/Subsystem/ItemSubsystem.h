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
 * CL/DE ���� ����ý����Դϴ�.
 * ���� ���� �� �������� �߱��ϰ�, ������ UID �� ������ ������ ���� ������ ĳ���մϴ�.
 * �ǹ������� ������ UID�� ���� �������� �߱�������, �� ��Ʈ������������ ���Ƿ� DE���� �߱��մϴ�.
 * PlayerState�� ��Ʈ��ũ ���� ����Ͽ� DE�� CL �� ������ ����ȭ�մϴ�.
 * CL������ ������ ������ ���� ���� �÷��̾��� PlayerUID�� 0���� �ϰ� �����մϴ�.
 */
UCLASS()
class COOPPUZZLE_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// �䱸 ������ ��� ������� �ϸ�, ���� ������ ������ �������� ������ ������ �õ�.
	void TryGiveItemToPlayer_CL( int64 iTargetPlayerUID, int64 iItemUID, int32 iItemCount );

	// ������ �߰�/���� ����. ���� �Ұ� �� ���� ������ ���� ���� false ��ȯ.
	bool AddItems_DE( int64 iPlayerUID, const TMap<FName/*ItemID*/, int32/*ItemCount*/>& mapItemInfos );

	// �䱸 ������ ��� ������� �ϸ�, �ϳ��� �����ϸ� false ��ȯ.
	bool HasItems( int64 iPlayerUID/*CL�� 0���� ����*/, const TMap<FName/*ItemID*/, int32/*ItemCount*/>& mapItemInfos) const;

	void GetItemID( int64 iItemUID, FName& ItemID ) const;
	int32 GetItemCount( int64 iItemUID ) const;
	UTexture2D* GetItemIcon( int64 iItemUID ) const;
	void GetPlayerInventoryItemUIDs( int64 iPlayerUID/*CL�� 0���� ����*/, TArray<int64>& arrItemUIDs, bool bSort ) const;

	// [����] �ܺο��� ���� ȣ������ ������. PlayerState�� �����Ͽ� ��Ʈ��ũ ���(DE �� CL)�� ���Ǵ� ��������Ʈ�Դϴ�.
	TMap<int64/*PlayerUID*/, FUpdateItemDelegate> OnUpdateInventoryItem_ToClient;

	// [����] �ܺο��� ���� ȣ������ ������. PlayerState�� �����Ͽ� ��Ʈ��ũ ���(CL �� DE)�� ���Ǵ� ��������Ʈ�Դϴ�. (TargetUID, ItemUID, ItemCount, SenderUID)
	FTransferItemDelegate OnTransferItemBetweenPlayers_ToServer;

protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

private:
	UFUNCTION()
	void UpdateItems( int64 iPlayerUID, const TArray<FItemSyncInfo>& arrUpdateItemInfos );

	UFUNCTION()
	void TransferItemBetweenPlayers( int64 iTargetPlayerUID, int64 iItemUID, int32 iItemCount, int64 iSenderPlayerUID );

	void LoadItemDataIfNotLoaded();

	// ���� �κ��丮/������ ���� ĳ�� ����������Դϴ�.
	// [�߿�] UpdateItems()������ ����, �ܺ� ���� ���� ����! (�ܺο��� ���� �� ������ ����ġ, ���� ������ �ֽ��ϴ�.)
	TMap<int64/*PlayerUID*/, TSet<int64/*ItemUID*/>> m_mapPlayerInventoryItems;
	TMap<int64/*ItemUID*/, int64/*PlayerUID*/> m_mapCachedInventoryItemOwners;
	TMap<FName/*ItemID*/, TSet<int64/*ItemUID*/>> m_mapGeneratedItemUIDs;
	TMap<int64/*ItemUID*/, TPair<FName/*ItemID*/, int32/*Count*/>> m_mapGeneratedItemInfos;

	// ���� ������ ������Ʈ ������ ������ ���̺� ��ü�� 1ȸ�� �ε�&ĳ���մϴ�. (Lazy Loading)
	TMap<FName, FItemDataRow*> m_mapCachedItemData;
};
