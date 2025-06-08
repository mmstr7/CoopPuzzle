// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "CoopPuzzle/Data/CooppuzzleStructs.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"

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
    UCoopPuzzleGameInstance* pGameInstance = static_cast< UCoopPuzzleGameInstance* >( GetGameInstance() );
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
        OnUpdateInventoryItem.FindOrAdd( 0 ).BindLambda( [this]( const TArray<FItemSyncInfo>& arrUpdateItemInfos )
        {
            UpdateItems( 0, arrUpdateItemInfos );
        } );
    }
}

void UItemSubsystem::UpdateItems( int64 iPlayerUID, const TArray<FItemSyncInfo>& arrUpdateItemInfos )
{
    TSet<int64/*ItemUID*/>& setPlayerInventoryItems = m_mapPlayerInventoryItems.FindOrAdd( iPlayerUID );

    for( const FItemSyncInfo& ItemInfo : arrUpdateItemInfos )
    {
        if( ItemInfo.Count <= 0 )
        {
            setPlayerInventoryItems.Remove( ItemInfo.ItemUID );
            m_mapCachedInventoryItemOwners.Remove( ItemInfo.ItemUID );
            m_mapGeneratedItemUIDs.FindOrAdd( ItemInfo.ItemID ).Remove( ItemInfo.ItemUID );
            m_mapGeneratedItemInfos.Remove( ItemInfo.ItemUID );
            // TODO: 삭제 이벤트/로깅 등
            continue;
        }

        setPlayerInventoryItems.Add( ItemInfo.ItemUID );
        m_mapCachedInventoryItemOwners.Add( ItemInfo.ItemUID, iPlayerUID );
        m_mapGeneratedItemUIDs.FindOrAdd( ItemInfo.ItemID ).Add( ItemInfo.ItemUID );
        m_mapGeneratedItemInfos.Add( ItemInfo.ItemUID, TPair<FName, int32>( ItemInfo.ItemID, ItemInfo.Count ) );
        // TODO: 추가/갱신 이벤트/로깅 등
    }
}
