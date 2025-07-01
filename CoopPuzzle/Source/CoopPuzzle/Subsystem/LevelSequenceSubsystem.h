// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleConstants.h"
#include "LevelSequenceSubsystem.generated.h"

DECLARE_DELEGATE_TwoParams( FPlayLevelSequenceDelegate, const FName&/*LevelSequenceID*/, int64/*LevelSequenceUID*/ );
DECLARE_DELEGATE_OneParam( FStopLevelSequenceDelegate, int64/*LevelSequenceUID*/ );
DECLARE_DELEGATE_TwoParams( FLevelSequenceFinishedDelegate, int64/*LevelSequenceUID*/, int64/*PlayerUID*/);

class ULevelSequencePlayer;
class UCoopPuzzleGameInstance;
struct FLevelSequenceDataRow;


struct FLevelSequenceSessionState
{
public:
	FName LevelSequenceID = FName();
	TSet<int64> setPlayingUsers;
	TSet<int64> setRequestedUsers;
	FDateTime TimeoutTime = FDateTime::MinValue();
};

USTRUCT()
struct FLevelSequenceClientState
{
	GENERATED_BODY()

public:
	bool bIsPlaying = false;
	int64 m_iPlayingLevelSequenceUID = INVALID_LEVELSEQUENCE_UID;

	UPROPERTY()
	TWeakObjectPtr<ULevelSequencePlayer> pLevelSequencePlayer = nullptr;
};

/**
 * 
 */
UCLASS()
class COOPPUZZLE_API ULevelSequenceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void PlayLevelSequence_DE( const FName& LevelSequenceID );
	void SkipLevelSequence_CL();

	// [����] �ܺο��� ���� ȣ������ ������. PlayerState�� �����Ͽ� ��Ʈ��ũ ���(DE �� CL)�� ���Ǵ� ��������Ʈ�Դϴ�.
	TMap<int64/*PlayerUID*/, FPlayLevelSequenceDelegate> OnPlayLevelSequence_ToClient;
	TMap<int64/*PlayerUID*/, FStopLevelSequenceDelegate> OnStopLevelSequence_ToClient;

	// [����] �ܺο��� ���� ȣ������ ������. PlayerState�� �����Ͽ� ��Ʈ��ũ ���(CL �� DE)�� ���Ǵ� ��������Ʈ�Դϴ�. (iLevelSequenceUID, iPlayerUID)
	FLevelSequenceFinishedDelegate OnLevelSequenceFinished_ToServer;
	
protected:
	virtual void Initialize( FSubsystemCollectionBase& Collection ) override;

private:
	void PlayLevelSequence( const FName& LevelSequenceID, int64 iLevelSequenceUID );
	void StopLevelSequence( int64 iLevelSequenceUID );

	UFUNCTION()
	void OnLevelSequenceFinished_CL();
	void OnLevelSequenceFinished_DE( int64 iLevelSequenceUID, int64 iPlayerUID );
	
	void LoadLevelSequenceDataIfNotLoaded();

	FLevelSequenceClientState m_LevelSequencePlayState_CL;

	TSet<int64/*PlayerUID*/> setLevelSequencePlayingUsers_DE;
	TMap<int64/*LevelSequenceUID*/, FLevelSequenceSessionState> mapLevelSequencePlayStates_DE;

	// ���� ���� ������ ��� ������ ������ ���̺� ��ü�� 1ȸ�� �ε�&ĳ���մϴ�. (Lazy Loading)
	TMap<FName, FLevelSequenceDataRow*> m_mapCachedLevelSequenceData;
};
