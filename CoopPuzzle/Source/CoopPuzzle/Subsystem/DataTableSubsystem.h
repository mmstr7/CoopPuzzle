// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "DataTableSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( LogDataTableSubsystem, Log, All );

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UDataTableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	template <class T>
	const T* GetDataRowOrNull( EDataTableType eDataTableType, const FName& RowName ) const
	{
		return DataTableMap.Contains( eDataTableType ) == true && IsValid( DataTableMap[eDataTableType] ) == true ? DataTableMap[eDataTableType]->FindRow<T>( RowName, nullptr ) : nullptr;
	}

public:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;
	
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = Setting  )
	TMap<EDataTableType, UDataTable*> DataTableMap;
};
