// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemSubsystem.generated.h"

DECLARE_DELEGATE_OneParam( FUpdateItemDelegate, const TArray<FItemSyncInfo>& );

struct FItemSyncInfo;
struct FItemDataRow;

/**
 * CL/DE ���� ����ý����Դϴ�.
 * ���� ���� �� �������� �߱��ϰ�, ������ UID �� ������ ������ ���� ������ ĳ���մϴ�.
 * �ǹ������� ������ UID�� ���� �������� �߱�������, �� ��Ʈ������������ ���Ƿ� DE���� �߱��մϴ�.
 * PlayerState�� ��Ʈ��ũ ���� ����Ͽ� DE�� CL �� ������ ����ȭ�մϴ�.
 * �� ��Ʈ������������ �������� ���� ����ϴ� ����� �����Ƿ�, CL������ �ڽ��� ������ ������ ��ȸ�� �� ������ ������ ���� ��û�� �� �� �����ϴ�.
 * CL������ ������ ������ ���� ���� �÷��̾��� PlayerUID�� 0���� �ϰ� �����մϴ�.
 */
UCLASS()
class COOPPUZZLE_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	int32 GetItemCount( int64 iItemUID ) const;
	UTexture2D* GetItemIcon( int64 iItemUID ) const;

	// CL ������ PlayerUID�� 0���� ����
	void GetPlayerInventoryItemUIDs( int64 iPlayerUID, TArray<int64>& arrItemUIDs, bool bSort );

	bool HasItems( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos ) const;

	// ������ �߰�/���� ����. ���� �Ұ� �� ���� ������ ���� ���� false ��ȯ.
	bool AddItems_DE( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos );

	// ������ ���� ����ȭ�� ��������Ʈ �Դϴ�. ���� ������Ʈ�� �ݵ�� WidgetDelegateSubsystem�� ���� ó���ϼ���.
	TMap<int64/*PlayerUID*/, FUpdateItemDelegate> OnUpdateInventoryItem_ToClient;

protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

private:
	UFUNCTION()
	void UpdateItems( int64 iPlayerUID, const TArray<FItemSyncInfo>& arrUpdateItemInfos );

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
