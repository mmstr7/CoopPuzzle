// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/ItemTooltip.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "Components/TextBlock.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"

void UItemTooltip::SetupTooltipWidget( const FName& ItemID )
{
	if( ItemID == m_CachedItemID )
		return;

	if( IsValid( ItemName ) == false || IsValid( Description ) == false )
		return;

	UDataTableSubsystem* pDataTableSubsystem = GetGameInstance()->GetSubsystem<UDataTableSubsystem>();
	checkf( IsValid( pDataTableSubsystem ) == true, TEXT( "DataTableSubsystem is not valid. Please check class." ) );

	const FItemDataRow* pItemData = pDataTableSubsystem->GetDataRowOrNull<FItemDataRow>( EDataTableType::Item, ItemID );
	checkf( pItemData != nullptr, TEXT( "ItemID '%s' is not valid. Please check Item data table." ), *ItemID.ToString() );

	ItemName->SetText( pItemData->ItemName );
	Description->SetText( pItemData->Description );
}
