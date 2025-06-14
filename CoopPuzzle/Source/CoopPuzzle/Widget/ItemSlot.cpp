// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/ItemSlot.h"
#include "CoopPuzzle/Data/CoopPuzzleUObjects.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CoopPuzzle/Subsystem/TooltipSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"

void UItemSlot::SetItemInfo( int64 iItemUID, bool bIsInit )
{
    UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
    checkf( IsValid( pItemSubsystem ) == true, TEXT( "pItemSubsystem is not valid." ) );

    if( bIsInit == true || iItemUID != m_iCachedItemUID )
    {
        if( IsValid( ItemIconImage ) == true )
        {
            UTexture2D* pItemIconTexture = pItemSubsystem->GetItemIcon( iItemUID );
            ItemIconImage->SetBrushFromTexture( pItemIconTexture );
            ItemIconImage->SetVisibility( IsValid( pItemIconTexture ) == true ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed );
        }

        m_iCachedItemUID = iItemUID;
        pItemSubsystem->GetItemID( iItemUID, m_CachedItemID );
    }

    if( IsValid( ItemCountText ) == true )
    {
        int32 iItemCount = pItemSubsystem->GetItemCount( iItemUID );
        ItemCountText->SetText( FText::AsNumber( iItemCount ) );
        ItemCountText->SetVisibility( iItemCount > 0 ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed );
    }
}

void UItemSlot::NativeOnListItemObjectSet( UObject* ListItemObject )
{
    UInventorySlotData* pSlotData = Cast<UInventorySlotData>( ListItemObject );
    
    SetItemInfo( IsValid( pSlotData ) == true ? pSlotData->ItemUID : INVALID_ITEM_UID, true );
}

UTooltipBase* UItemSlot::GetItemTooltipWidget()
{
    if( m_iCachedItemUID == INVALID_ITEM_UID )
        return nullptr;

    UTooltipSubsystem* pTooltipSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UTooltipSubsystem>() : nullptr;
    checkf( IsValid( pTooltipSubsystem ) == true, TEXT( "pTooltipSubsystem is not valid." ) );

    return pTooltipSubsystem->GetTooltipWidget( ETooltipType::Item, m_CachedItemID );
}
