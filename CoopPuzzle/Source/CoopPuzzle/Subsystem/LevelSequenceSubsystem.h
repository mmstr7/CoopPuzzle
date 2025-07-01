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

	// [주의] 외부에서 직접 호출하지 마세요. PlayerState를 경유하여 네트워크 통신(DE → CL)에 사용되는 델리게이트입니다.
	TMap<int64/*PlayerUID*/, FPlayLevelSequenceDelegate> OnPlayLevelSequence_ToClient;
	TMap<int64/*PlayerUID*/, FStopLevelSequenceDelegate> OnStopLevelSequence_ToClient;

	// [주의] 외부에서 직접 호출하지 마세요. PlayerState를 경유하여 네트워크 통신(CL → DE)에 사용되는 델리게이트입니다. (iLevelSequenceUID, iPlayerUID)
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

	// 최초 레벨 시퀀스 재생 시점에 데이터 테이블 전체를 1회만 로드&캐싱합니다. (Lazy Loading)
	TMap<FName, FLevelSequenceDataRow*> m_mapCachedLevelSequenceData;
};
