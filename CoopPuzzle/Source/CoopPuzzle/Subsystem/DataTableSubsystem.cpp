// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"

DEFINE_LOG_CATEGORY( LogDataTableSubsystem );

void UDataTableSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Super::Initialize( Collection );

	UE_LOG( LogDataTableSubsystem, Log, TEXT( "DataTableSubsystem Initialized." ) );

	for( uint8 i = 0; i < static_cast< uint8 >( EDataTableType::MAX ); ++i )
	{
		EDataTableType eDataTableType = static_cast< EDataTableType >( i );
		if( DataTableMap.Contains( eDataTableType ) == false || IsValid( DataTableMap[eDataTableType] ) == false )
			UE_LOG( LogDataTableSubsystem, Warning, TEXT( "DataTableMap missing entry for [%s]. Check DataTableSubsystem." ), *UEnum::GetValueAsString( eDataTableType ) );
	}
}