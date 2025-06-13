// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CoopPuzzleGameInstance.generated.h"

class ACoopPuzzleCharacter;

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API UCoopPuzzleGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	int64 GenerateUID_DE();

	// DE�� ACoopPuzzleCharacter::BeginPlay() ������ ���õ�. CL���� �� ����ȭ ���� ��ȿ.
	int64 GetLocalPlayerUID_CL();

	bool IsLocalPlayer_CL( const ACoopPuzzleCharacter* pPlayer );

private:
	int64 m_iNextUID_DE = 0;
};
