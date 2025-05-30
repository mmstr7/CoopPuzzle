// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleData.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "DataTableSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN( LogDataTableSubsystem, Log, All );

/**
 * CL/DE 공용 서브시스템입니다.
 * 데이터 테이블에 대한 빠르고 간편한 접근을 제공합니다.
 * 초기화 시, Enum 기반으로 지정된 경로의 모든 데이터 테이블을 자동으로 로드합니다.
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

protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;
	
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = Setting  )
	TMap<EDataTableType, UDataTable*> DataTableMap;
};
