// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemSubsystem.generated.h"

DECLARE_DELEGATE_OneParam( FUpdateItemDelegate, const TArray<FItemSyncInfo>& );

struct FItemSyncInfo;

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
	bool HasItems( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos ) const;

	// ������ �߰�/���� ����. ���� �Ұ� �� ���� ������ ���� ���� false ��ȯ.
	bool AddItems_DE( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos );

	// DE������ PlayerState��, CL������ �ش� Ŭ������ UpdateItems�� ���ε��մϴ�.
	// CL������ PlayerUID�� 0���� �����մϴ�.
	// (���� ������Ʈ�� �ݵ�� WidgetDelegateSubsystem�� ���� ó���ϼ���.)
	TMap<int64/*PlayerUID*/, FUpdateItemDelegate> OnUpdateInventoryItem;

protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

private:
	UFUNCTION()
	void UpdateItems( int64 iPlayerUID, const TArray<FItemSyncInfo>& arrUpdateItemInfos );

	// ���� �κ��丮/������ ���� ĳ�� ����������Դϴ�.
	// [�߿�] UpdateItems()������ ����, �ܺ� ���� ���� ����! (�ܺο��� ���� �� ������ ����ġ, ���� ������ �ֽ��ϴ�.)
	TMap<int64/*PlayerUID*/, TSet<int64/*ItemUID*/>> m_mapPlayerInventoryItems;
	TMap<int64/*ItemUID*/, int64/*PlayerUID*/> m_mapCachedInventoryItemOwners;
	TMap<FName/*ItemID*/, TSet<int64/*ItemUID*/>> m_mapGeneratedItemUIDs;
	TMap<int64/*ItemUID*/, TPair<FName/*ItemID*/, int32/*Count*/>> m_mapGeneratedItemInfos;
};
