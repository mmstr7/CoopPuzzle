// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"

DEFINE_LOG_CATEGORY( LogDataTableSubsystem );

const UDataTable* UDataTableSubsystem::GetDataTable( EDataTableType eDataTableType ) const
{
    return *DataTableMap.Find( eDataTableType );
}

void UDataTableSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Super::Initialize( Collection );

	UE_LOG( LogDataTableSubsystem, Log, TEXT( "DataTableSubsystem Initialized." ) );

    UEnum* pEnum = StaticEnum<EDataTableType>();
    for( uint8 i = 0; i < static_cast<uint8>( EDataTableType::MAX ); ++i )
    {
        FString sTableName = pEnum->GetNameStringByIndex( i ) + TEXT( "DataTable" );
        UDataTable* pLoadedTable = LoadObject<UDataTable>( nullptr, *FString::Printf( TEXT( "/Game/TopDown/DataTable/%s.%s" ), *sTableName, *sTableName ) );
        checkf( IsValid( pLoadedTable ) == true, TEXT( "[%s] is not valid. Please check EDataTableType or DataTable path" ), *sTableName );

        DataTableMap.Add( static_cast<EDataTableType>( i ), pLoadedTable );
    }
}