// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"

void UCoopPuzzleGameInstance::Init()
{
	Super::Init();
}

int64 UCoopPuzzleGameInstance::GenerateUID_DE()
{
	checkf( IsDedicatedServerInstance() == true, TEXT( "UID must not be generated on client." ) );

	return m_iNextUID++;
}
