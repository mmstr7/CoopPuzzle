// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "CoopPuzzle/Data/CoopPuzzleStructs.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Subsystem/PlayerManagerSubsystem.h"

void UItemSubsystem::TryGiveItemToPlayer_CL( int64 iTargetPlayerUID, int64 iItemUID, int32 iItemCount )
{
    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    checkf( IsValid( pGameInstance ) == true, TEXT( "pGameInstance is not valid." ) );
    checkf( pGameInstance->IsDedicatedServerInstance() == false, TEXT( "CL Only." ) );

    TransferItemBetweenPlayers( iTargetPlayerUID, iItemUID, iItemCount, pGameInstance->GetLocalPlayerUID_CL() );
}

bool UItemSubsystem::AddItems_DE( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos )
{
    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    checkf( IsValid( pGameInstance ) == true, TEXT( "pGameInstance is not valid." ) );
    checkf( pGameInstance->IsDedicatedServerInstance() == true, TEXT( "DE Only." ) );

    TArray<FItemSyncInfo> arrUpdateItemInfos;
    TSet<int64/*ItemUID*/>& setPlayerInventoryItems = m_mapPlayerInventoryItems.FindOrAdd( iPlayerUID );

    for( const TPair<FName/*ItemID*/, int32/*Count*/>& ItemToAdd : mapItemInfos )
    {
        checkf( ItemToAdd.Value != 0, TEXT( "Item count must not be zero. (ItemID: %s)" ), *ItemToAdd.Key.ToString() );

        int64 iItemUID = -1;

        for( const int64& iGeneratedItemUID : m_mapGeneratedItemUIDs.FindOrAdd( ItemToAdd.Key ) )
        {
            if( setPlayerInventoryItems.Contains( iGeneratedItemUID ) == true )
            {
                iItemUID = iGeneratedItemUID;
                break;
            }
        }

        if( iItemUID > 0 )
        {
            int32 iUpdateItemCount = m_mapGeneratedItemInfos.FindRef( iItemUID ).Value + ItemToAdd.Value;
            if( iUpdateItemCount < 0 )
                return false;

            arrUpdateItemInfos.Add( FItemSyncInfo( iItemUID, ItemToAdd.Key, iUpdateItemCount ) );
        }
        else
        {
            if( ItemToAdd.Value < 0 )
                return false;

            arrUpdateItemInfos.Add( FItemSyncInfo( pGameInstance->GenerateUID_DE(), ItemToAdd.Key, ItemToAdd.Value ) );
        }
    }

    UpdateItems( iPlayerUID, arrUpdateItemInfos );
    return true;
}

bool UItemSubsystem::HasItems( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos ) const
{
    for( const TPair<FName, int32>& pairItemInfo : mapItemInfos )
    {
        const FName& ItemID = pairItemInfo.Key;
        int32 iItemCount = pairItemInfo.Value;
        checkf( iItemCount > 0, TEXT( "Item count must be greater than zero. (ItemID: %s)" ), *ItemID.ToString() );

        const TSet<int64>* pGeneratedItemUIDs = m_mapGeneratedItemUIDs.Find( ItemID );
        if( pGeneratedItemUIDs == nullptr )
            return false;

        int64 iFindedItemUID = -1;
        for( const int64& iItemUID : *pGeneratedItemUIDs )
        {
            const int64* iItemOwnerUID = m_mapCachedInventoryItemOwners.Find( iItemUID );
            if( iItemOwnerUID != nullptr && *iItemOwnerUID == iPlayerUID )
            {
                iFindedItemUID = iItemUID;
                break;
            }
        }

        if( iFindedItemUID < 0 )
            return false;

        if( m_mapGeneratedItemInfos.FindRef( iFindedItemUID ).Value < iItemCount )
            return false;
    }

    return true;
}

void UItemSubsystem::GetItemID( int64 iItemUID, FName& ItemID ) const
{
    ItemID = m_mapGeneratedItemInfos.FindRef( iItemUID ).Key;
}

int32 UItemSubsystem::GetItemCount( int64 iItemUID ) const
{
    return m_mapGeneratedItemInfos.FindRef( iItemUID ).Value;
}

UTexture2D* UItemSubsystem::GetItemIcon( int64 iItemUID ) const
{
    const auto* pItemInfo = m_mapGeneratedItemInfos.Find( iItemUID );
    if( pItemInfo == nullptr )
        return nullptr;

    const FItemDataRow* pItemData = m_mapCachedItemData.FindRef( pItemInfo->Key );
    if( pItemData == nullptr )
        return nullptr;

    return pItemData->ItemIcon;
}

void UItemSubsystem::GetPlayerInventoryItemUIDs( int64 iPlayerUID, TArray<int64>& arrItemUIDs, bool bSort ) const
{
	arrItemUIDs = m_mapPlayerInventoryItems.FindRef( iPlayerUID ).Array();

	if( bSort == true )
	{
		arrItemUIDs.Sort();
	}
}

void UItemSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
    Super::Initialize( Collection );

    if( IsValid( GetGameInstance() ) == false )
        return;

    if( GetGameInstance()->IsDedicatedServerInstance() == true )
    {
        // DE
        OnTransferItemBetweenPlayers_ToServer.BindUObject( this, &UItemSubsystem::TransferItemBetweenPlayers );
    }
    else
    {
        // CL
        OnUpdateInventoryItem_ToClient.FindOrAdd( 0 ).BindLambda( [this]( const TArray<FItemSyncInfo>& arrUpdateItemInfos )
        {
            UpdateItems( 0, arrUpdateItemInfos );
        } );
    }
}

void UItemSubsystem::UpdateItems( int64 iPlayerUID, const TArray<FItemSyncInfo>& arrUpdateItemInfos )
{
    LoadItemDataIfNotLoaded();

    UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>();
    bool bIsWidgetDelegatesValid = IsValid( pWidgetDelegateSubsystem ) == true;

    TSet<int64/*ItemUID*/>& setPlayerInventoryItems = m_mapPlayerInventoryItems.FindOrAdd( iPlayerUID );

    for( const FItemSyncInfo& ItemInfo : arrUpdateItemInfos )
    {
        checkf( m_mapCachedItemData.Contains( ItemInfo.ItemID ) == true, TEXT( "ItemID '%s' is not valid. Please check ItemDataTable." ), *ItemInfo.ItemID.ToString() );

        if( ItemInfo.Count <= 0 )
        {
            setPlayerInventoryItems.Remove( ItemInfo.ItemUID );
            m_mapCachedInventoryItemOwners.Remove( ItemInfo.ItemUID );
            m_mapGeneratedItemUIDs.FindOrAdd( ItemInfo.ItemID ).Remove( ItemInfo.ItemUID );
            m_mapGeneratedItemInfos.Remove( ItemInfo.ItemUID );
            continue;
        }

        setPlayerInventoryItems.Add( ItemInfo.ItemUID );
        m_mapCachedInventoryItemOwners.Add( ItemInfo.ItemUID, iPlayerUID );
        m_mapGeneratedItemUIDs.FindOrAdd( ItemInfo.ItemID ).Add( ItemInfo.ItemUID );
        m_mapGeneratedItemInfos.Add( ItemInfo.ItemUID, TPair<FName, int32>( ItemInfo.ItemID, ItemInfo.Count ) );
    }

    if( IsValid( GetGameInstance() ) == false )
        return;

    if( GetGameInstance()->IsDedicatedServerInstance() == true )
    {
        OnUpdateInventoryItem_ToClient.FindOrAdd( iPlayerUID ).ExecuteIfBound( arrUpdateItemInfos );
    }
    else
    {
        if( bIsWidgetDelegatesValid == true )
        {
            pWidgetDelegateSubsystem->OnPlayerInventoryUpdated_ToClient.FindOrAdd( 0 ).Broadcast();
        }
    }
}

void UItemSubsystem::TransferItemBetweenPlayers( int64 iTargetPlayerUID, int64 iItemUID, int32 iItemCount, int64 iSenderPlayerUID )
{
    if( iTargetPlayerUID == iSenderPlayerUID )
        return;

    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    checkf( IsValid( pGameInstance ) == true, TEXT( "pGameInstance is not valid." ) );

    checkf( iItemUID != INVALID_ITEM_UID, TEXT( "iItemUID is not valid." ) );
    checkf( iTargetPlayerUID != INVALID_PLAYER_UID, TEXT( "iTargetPlayerUID is not valid." ) );
    checkf( iSenderPlayerUID != INVALID_PLAYER_UID, TEXT( "iSenderPlayerUID is not valid." ) );
    checkf( iItemCount > 0, TEXT( "Item count must be greater than zero. (iItemUID: %d)" ), iItemUID );

    const int64* pItemOwnerUID = m_mapCachedInventoryItemOwners.Find( iItemUID );
    if( pItemOwnerUID == nullptr )
        return;

    // 전달하려는 아이템의 실제 주인 확인
    int64 iExpectedOwnerUID = pGameInstance->IsDedicatedServerInstance() ? iSenderPlayerUID : 0/*CL은 0으로 통일*/;
    if( *pItemOwnerUID != iExpectedOwnerUID )
        return;

    // 아이템 개수 체크
    const auto* pItemInfo = m_mapGeneratedItemInfos.Find( iItemUID );
    if( pItemInfo == nullptr || pItemInfo->Value < iItemCount )
        return;

    if( pGameInstance->IsDedicatedServerInstance() == true )
    {
        UPlayerManagerSubsystem* pPlayerManagerSubsystem = pGameInstance->GetSubsystem<UPlayerManagerSubsystem>();
        if( IsValid( pPlayerManagerSubsystem ) == false )
            return;

        // 아이템 전달 거리가 부족할 경우 보내는 자 위젯에 알리고 실패
        if( pPlayerManagerSubsystem->IsPlayersInRange( iTargetPlayerUID, iSenderPlayerUID, 100.f ) == false )
        {
            UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = pGameInstance->GetSubsystem<UWidgetDelegateSubsystem>();
            if( IsValid( pWidgetDelegateSubsystem ) == false )
                return;

			FText FailText = FText::FromStringTable( FName( TEXT( "/Game/TopDown/DataTable/GlobalStringTable" ) ), TEXT( "Item_Transfer_Fail_OutOfRange" ) );
			pWidgetDelegateSubsystem->OnShowLocalNotification_ToClient.FindOrAdd( iSenderPlayerUID ).Broadcast( FailText );
            return;
        }

        // 보내는 유저 아이템 수량 감소
        UpdateItems( iSenderPlayerUID, { FItemSyncInfo( iItemUID, pItemInfo->Key, pItemInfo->Value - iItemCount ) } );

        // 받는 유저 아이템 추가
        AddItems_DE( iTargetPlayerUID, { { pItemInfo->Key, iItemCount } } );
    }
    else
    {
        // CL
        OnTransferItemBetweenPlayers_ToServer.ExecuteIfBound( iTargetPlayerUID, iItemUID, iItemCount, iSenderPlayerUID );
    }
}

void UItemSubsystem::LoadItemDataIfNotLoaded()
{
    if( m_mapCachedItemData.Num() > 0 )
        return;

    UDataTableSubsystem* pDataTableSubsystem = IsValid(GetGameInstance()) == true ? GetGameInstance()->GetSubsystem<UDataTableSubsystem>() : nullptr;
    checkf( IsValid( pDataTableSubsystem ) == true, TEXT( "DataTableSubsystem is not valid. Please check class." ) );

    const UDataTable* pItemDataTable = pDataTableSubsystem->GetDataTable( EDataTableType::Item );
    checkf( IsValid( pItemDataTable ) == true, TEXT( "pItemDataTable is not valid." ) );

    for( const auto& RowPair : pItemDataTable->GetRowMap() )
    {
        m_mapCachedItemData.Add( RowPair.Key, reinterpret_cast<FItemDataRow*>( RowPair.Value ) );
    }
}
