// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"
#include "CoopPuzzle/Subsystem/EventTriggerManagerSubsystem.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"

int64 UCoopPuzzleGameInstance::GenerateUID_DE()
{
	checkf( IsDedicatedServerInstance() == true, TEXT( "UID must not be generated on client." ) );

	return m_iNextUID_DE++;
}

int64 UCoopPuzzleGameInstance::GetLocalPlayerUID_CL()
{
	checkf( IsDedicatedServerInstance() == false, TEXT( "CL Only." ) );

	APlayerController* pPlayerController = IsValid( GetWorld() ) == true ? GetWorld()->GetFirstPlayerController() : nullptr;
	if( IsValid( pPlayerController ) == false )
		return INVALID_PLAYER_UID;

	ACoopPuzzleCharacter* pLocalPlayer = Cast<ACoopPuzzleCharacter>( pPlayerController->GetPawn() );
	if( IsValid( pLocalPlayer ) == false )
		return INVALID_PLAYER_UID;

	return pLocalPlayer->GetPlayerUID();
}

bool UCoopPuzzleGameInstance::IsLocalPlayer_CL( const ACoopPuzzleCharacter* pPlayer )
{
	checkf( IsDedicatedServerInstance() == false, TEXT( "CL Only." ) );

	return IsValid( pPlayer ) == true && pPlayer->GetPlayerUID() == GetLocalPlayerUID_CL();
}
