// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/InventoryPanel.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Subsystem/ItemSubsystem.h"
#include "Components/TileView.h"
#include "CoopPuzzle/Data/CoopPuzzleUObjects.h"
#include "CoopPuzzle/Widget/ItemSlot.h"
#include "CoopPuzzle/Widget/NotificationText.h"

void UInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnPlayerInventoryUpdated_ToClient.FindOrAdd( 0 ).AddUObject( this, &UInventoryPanel::UpdateInventoryPanel );
		pWidgetDelegateSubsystem->OnLevelSequenceStateChanged.AddUObject( this, &UInventoryPanel::LevelSequenceStateChanged );
		pWidgetDelegateSubsystem->OnShowItemNotifications_ToClient.FindOrAdd( 0 ).AddUObject( this, &UInventoryPanel::AddItemNotifications );
	}

	UpdateInventoryPanel();
	LevelSequenceStateChanged( EProcessState::Finished );
}

void UInventoryPanel::NativeDestruct()
{
	Super::NativeDestruct();

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnPlayerInventoryUpdated_ToClient.Remove( 0 );
		pWidgetDelegateSubsystem->OnLevelSequenceStateChanged.RemoveAll( this );
		pWidgetDelegateSubsystem->OnShowItemNotifications_ToClient.Remove( 0 );
	}
}

void UInventoryPanel::UpdateInventoryPanel()
{
	UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
	if( IsValid( pItemSubsystem ) == false )
		return;

	checkf( IsValid( ItemList ) == true, TEXT( "ItemList is not valid." ) );
	
	TArray<UUserWidget*> arrDisplayedWidgets = ItemList->GetDisplayedEntryWidgets();

	TArray<int64> arrSortedItemUIDs;
	pItemSubsystem->GetPlayerInventoryItemUIDs( 0, arrSortedItemUIDs, true );

	for( int i = 0; i < FMath::Max( arrSortedItemUIDs.Num(), m_iDefaultSlotCount ); ++i )
	{
		int64 iItemUID = arrSortedItemUIDs.IsValidIndex( i ) == true ? arrSortedItemUIDs[i] : -1;

		if( arrDisplayedWidgets.IsValidIndex( i ) == true )
		{
			UItemSlot* pItemSlot = Cast<UItemSlot>( arrDisplayedWidgets[i] );
			checkf( IsValid( pItemSlot ) == true, TEXT( "pItemSlot is not valid." ) );

			pItemSlot->SetItemInfo( iItemUID, false );
		}
		else
		{
			UInventorySlotData* pItemSlotData = NewObject<UInventorySlotData>();
			pItemSlotData->Init( iItemUID );
			ItemList->AddItem( pItemSlotData );
		}
	}

	// 보여줄 데이터 개수 초과된 슬롯은 제거
	while( ItemList->GetNumItems() > FMath::Max( arrSortedItemUIDs.Num(), m_iDefaultSlotCount ) )
	{
		ItemList->RemoveItem( ItemList->GetItemAt( ItemList->GetNumItems() - 1 ) );
	}
}

void UInventoryPanel::LevelSequenceStateChanged( EProcessState eState )
{
	SetVisibility( eState == EProcessState::Finished ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed );
}

void UInventoryPanel::AddItemNotifications( const TArray<FItemNotifyInfo>& arrNotifications )
{
	if( IsValid( ItemUpdateNotification ) == false )
		return;

	UItemSubsystem* pItemSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UItemSubsystem>() : nullptr;
	if( IsValid( pItemSubsystem ) == false )
		return;

	UEnum* pEnum = StaticEnum<EItemNotification>();
	if( IsValid( pEnum ) == false )
		return;

	for( const FItemNotifyInfo& Info : arrNotifications )
	{
		FText ItemName;
		pItemSubsystem->GetItemName( Info.ItemID, ItemName );

		FString EnumString = FString::Printf( TEXT( "%s::%s" ), *pEnum->GetName(), *pEnum->GetNameStringByValue( ( int64 )Info.ItemNotificationType ) );
		FText FormatText = FText::FromStringTable( FName( TEXT( "/Game/TopDown/DataTable/EnumStringTable" ) ), EnumString );

		UE_LOG( LogTemp, Error, TEXT( "%s" ), *EnumString );

		if( ItemName.IsEmpty() == true || FormatText.IsEmpty() == true )
			continue;

		ItemUpdateNotification->AddNotification( FText::Format( FormatText, FFormatNamedArguments{ { TEXT( "ItemName" ), ItemName } } ) );
	}
}
