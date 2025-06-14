// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "CoopPuzzle/Data/CoopPuzzleStructs.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"

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

void UItemSubsystem::GetPlayerInventoryItemUIDs( int64 iPlayerUID, TArray<int64>& arrItemUIDs, bool bSort )
{
    arrItemUIDs = m_mapPlayerInventoryItems.FindRef( iPlayerUID ).Array();

    if( bSort == true )
    {
        arrItemUIDs.Sort();
    }
}

bool UItemSubsystem::HasItems( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos ) const
{
    for( const TPair<FName, int32>& ItemInfo : mapItemInfos )
    {
        checkf( ItemInfo.Value > 0, TEXT( "Item count must be greater than zero. (ItemID: %s)" ), *ItemInfo.Key.ToString() );

        const TSet<int64>* pGeneratedItemUIDs = m_mapGeneratedItemUIDs.Find(ItemInfo.Key);
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

        if( m_mapGeneratedItemInfos.FindRef( iFindedItemUID ).Value < ItemInfo.Value )
            return false;
    }
    
    return true;
}

bool UItemSubsystem::AddItems_DE( int64 iPlayerUID, const TMap<FName, int32>& mapItemInfos )
{
    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    checkf( IsValid( pGameInstance ) == true, TEXT( "pGameInstance is not valid." ) );

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

void UItemSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
    Super::Initialize( Collection );

    if( IsValid( GetGameInstance() ) == false )
        return;

    if( GetGameInstance()->IsDedicatedServerInstance() == false )
    {
        OnUpdateInventoryItem_ToClient.FindOrAdd( 0 ).BindLambda( [this]( const TArray<FItemSyncInfo>& arrUpdateItemInfos )
        {
            UpdateItems( 0, arrUpdateItemInfos );
        } );
    }
}

void UItemSubsystem::UpdateItems( int64 iPlayerUID, const TArray<FItemSyncInfo>& arrUpdateItemInfos )
{
    LoadItemDataIfNotLoaded();

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
        UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>();
        if( IsValid( pWidgetDelegateSubsystem ) == true )
        {
            pWidgetDelegateSubsystem->OnPlayerInventoryUpdated_ToClient.FindOrAdd( 0 ).Broadcast();
        }
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
