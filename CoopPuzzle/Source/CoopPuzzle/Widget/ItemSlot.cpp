// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/ItemSlot.h"
#include "CoopPuzzle/Data/CoopPuzzleUObjects.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CoopPuzzle/Subsystem/TooltipSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CoopPuzzle/Object/ItemDragDropOperation.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"

void UItemSlot::SetItemInfo( int64 iItemUID, bool bIsInit )
{
    UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
    checkf( IsValid( pItemSubsystem ) == true, TEXT( "pItemSubsystem is not valid." ) );

    if( bIsInit == true || iItemUID != m_iCachedItemUID )
    {
        if( IsValid( ItemIconImage ) == true )
        {
            UTexture2D* pItemIconTexture = pItemSubsystem->GetItemIcon( iItemUID );
            ItemIconImage->SetBrushFromTexture( pItemIconTexture, true );
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

FReply UItemSlot::NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent )
{
    // 아이템 유효할 경우 좌클릭 시 드래그 감지
    if( m_iCachedItemUID != INVALID_ITEM_UID && InMouseEvent.IsMouseButtonDown( EKeys::LeftMouseButton ) == true )
        return UWidgetBlueprintLibrary::DetectDragIfPressed( InMouseEvent, this, EKeys::LeftMouseButton ).NativeReply;

    return Super::NativeOnMouseButtonDown( InGeometry, InMouseEvent );
}

void UItemSlot::NativeOnDragDetected( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation )
{
    Super::NativeOnDragDetected( InGeometry, InMouseEvent, OutOperation );

    if( m_iCachedItemUID == INVALID_ITEM_UID || IsValid( ItemIconImage ) == false )
        return;

    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    if( IsValid( pGameInstance ) == false )
        return;

    UItemDragDropOperation* pItemDragDropOperation = NewObject<UItemDragDropOperation>( this );
    if( IsValid( pItemDragDropOperation ) == false )
        return;

    // 드래그한 아이템 정보 세팅
    pItemDragDropOperation->ItemUID = m_iCachedItemUID;
    pItemDragDropOperation->DefaultDragVisual = ItemIconImage;

    OutOperation = pItemDragDropOperation;
}

UTooltipBase* UItemSlot::GetItemTooltipWidget()
{
    if( m_iCachedItemUID == INVALID_ITEM_UID )
        return nullptr;

    UTooltipSubsystem* pTooltipSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UTooltipSubsystem>() : nullptr;
    checkf( IsValid( pTooltipSubsystem ) == true, TEXT( "pTooltipSubsystem is not valid." ) );

    return pTooltipSubsystem->GetTooltipWidget( ETooltipType::Item, m_CachedItemID );
}
