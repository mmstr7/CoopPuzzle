// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/LevelSequenceSubsystem.h"
#include "CoopPuzzle/Subsystem/DataTableSubsystem.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "CoopPuzzle/Game/CoopPuzzleGameInstance.h"

void ULevelSequenceSubsystem::PlayLevelSequence_DE( const FName& LevelSequenceID )
{
    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    if( IsValid( pGameInstance ) == false )
        return;

    checkf( pGameInstance->IsDedicatedServerInstance() == true, TEXT( "DE Only." ) );

	PlayLevelSequence( LevelSequenceID, pGameInstance->GenerateUID_DE() );
}

void ULevelSequenceSubsystem::SkipLevelSequence_CL()
{
    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    if( IsValid( pGameInstance ) == false )
        return;

    checkf( pGameInstance->IsDedicatedServerInstance() == false, TEXT( "CL Only." ) );

	if( m_LevelSequencePlayState_CL.bIsPlaying == false )
		return;

    OnLevelSequenceFinished_ToServer.ExecuteIfBound( m_LevelSequencePlayState_CL.m_iPlayingLevelSequenceUID, pGameInstance->GetLocalPlayerUID_CL() );
}

void ULevelSequenceSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
    Super::Initialize( Collection );

    if( IsValid( GetGameInstance() ) == false )
        return;

    if( GetGameInstance()->IsDedicatedServerInstance() == true )
    {
        // DE
        OnLevelSequenceFinished_ToServer.BindUObject( this, &ULevelSequenceSubsystem::OnLevelSequenceFinished_DE );
    }
    else
    {
        // CL
        OnPlayLevelSequence_ToClient.FindOrAdd( 0 ).BindUObject( this, &ULevelSequenceSubsystem::PlayLevelSequence );
        OnStopLevelSequence_ToClient.FindOrAdd( 0 ).BindUObject( this, &ULevelSequenceSubsystem::StopLevelSequence );
    }
}

void ULevelSequenceSubsystem::PlayLevelSequence( const FName& LevelSequenceID, int64 iLevelSequenceUID )
{
    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    if( IsValid( pGameInstance ) == false )
        return;

    LoadLevelSequenceDataIfNotLoaded();

    FLevelSequenceDataRow* pLevelSequenceData = m_mapCachedLevelSequenceData.FindRef( LevelSequenceID );
    checkf( pLevelSequenceData != nullptr, TEXT( "LevelSequenceData '%s' is not valid. Please check data table." ), *LevelSequenceID.ToString() );
    checkf( IsValid( pLevelSequenceData->LevelSequence ) == true, TEXT( "LevelSequence '%s' is not valid. Please check data table." ), *LevelSequenceID.ToString() );

    if( pGameInstance->IsDedicatedServerInstance() == true )
    {
        // DE
        if( OnPlayLevelSequence_ToClient.Num() <= 0 || OnPlayLevelSequence_ToClient.Num() <= setLevelSequencePlayingUsers_DE.Num() )
            return;

        UMovieScene* pMovieScene = pLevelSequenceData->LevelSequence->GetMovieScene();
        if( IsValid( pMovieScene ) == false )
            return;

		FFrameRate TickResolution = pMovieScene->GetTickResolution();
		float fDuration = TickResolution.AsSeconds( pMovieScene->GetPlaybackRange().GetUpperBoundValue() ) - TickResolution.AsSeconds( pMovieScene->GetPlaybackRange().GetLowerBoundValue() );
        if( fDuration <= 0.f )
            return;

        FLevelSequenceSessionState& refSequencePlayState = mapLevelSequencePlayStates_DE.FindOrAdd( iLevelSequenceUID );
        refSequencePlayState.LevelSequenceID = LevelSequenceID;
        refSequencePlayState.TimeoutTime = FDateTime::Now() + FTimespan::FromSeconds( fDuration ) + FTimespan::FromSeconds( TIMEOUT_OFFSET );

        for( const TPair<int64, FPlayLevelSequenceDelegate>& LevelSequenceDelegates : OnPlayLevelSequence_ToClient )
        {
            int64 iPlayerUID = LevelSequenceDelegates.Key;

            if( setLevelSequencePlayingUsers_DE.Contains( iPlayerUID ) == true )
                continue;

            if( LevelSequenceDelegates.Value.ExecuteIfBound( LevelSequenceID, iLevelSequenceUID ) == false )
                continue;

            // 레벨시퀀스 재생 요청한 유저 캐싱
            refSequencePlayState.setPlayingUsers.Add( iPlayerUID );
            refSequencePlayState.setRequestedUsers.Add( iPlayerUID );
            setLevelSequencePlayingUsers_DE.Add( iPlayerUID );
        }
    }
    else
    {
        // 레벨 시퀀스 실행 불가 상태라면 즉시 종료 알림
        if( m_LevelSequencePlayState_CL.bIsPlaying == true )
        {
            OnLevelSequenceFinished_ToServer.ExecuteIfBound( iLevelSequenceUID, pGameInstance->GetLocalPlayerUID_CL() );
            return;
        }
        
        FMovieSceneSequencePlaybackSettings PlaybackSettings;
        PlaybackSettings.bAutoPlay = true;

        ALevelSequenceActor* pLevelSequenceActor = nullptr;
        ULevelSequencePlayer* pLevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer( GetWorld(), pLevelSequenceData->LevelSequence, PlaybackSettings, pLevelSequenceActor );
        if( IsValid( pLevelSequencePlayer ) == false )
        {
            OnLevelSequenceFinished_ToServer.ExecuteIfBound( iLevelSequenceUID, pGameInstance->GetLocalPlayerUID_CL() );
            return;
        }

        m_LevelSequencePlayState_CL.bIsPlaying = true;
        m_LevelSequencePlayState_CL.pLevelSequencePlayer = pLevelSequencePlayer;
        m_LevelSequencePlayState_CL.m_iPlayingLevelSequenceUID = iLevelSequenceUID;

        pLevelSequencePlayer->Play();
        pLevelSequencePlayer->OnFinished.AddDynamic( this, &ULevelSequenceSubsystem::OnLevelSequenceFinished_CL );
    }
}

void ULevelSequenceSubsystem::StopLevelSequence( int64 iLevelSequenceUID )
{
    if( IsValid( GetGameInstance() ) == false )
        return;

    if( GetGameInstance()->IsDedicatedServerInstance() == true )
    {
        auto* pLevelSequencePlayStates = mapLevelSequencePlayStates_DE.Find( iLevelSequenceUID );
        if( pLevelSequencePlayStates == nullptr )
            return;

        for( int64 iFinishPlayerUID : pLevelSequencePlayStates->setRequestedUsers )
        {
            OnStopLevelSequence_ToClient.FindRef( iFinishPlayerUID ).ExecuteIfBound( iLevelSequenceUID );
            setLevelSequencePlayingUsers_DE.Remove( iFinishPlayerUID );
        }

        mapLevelSequencePlayStates_DE.Remove( iLevelSequenceUID );
    }
    else
    {
        if( m_LevelSequencePlayState_CL.bIsPlaying == false )
            return;

        checkf( m_LevelSequencePlayState_CL.m_iPlayingLevelSequenceUID == iLevelSequenceUID, TEXT( "LevelSequenceUID is not matching. please check logic." ) );

        ULevelSequencePlayer* pLevelSequencePlayer = m_LevelSequencePlayState_CL.pLevelSequencePlayer.Get();
        if( IsValid( pLevelSequencePlayer ) == true )
        {
            pLevelSequencePlayer->OnFinished.RemoveAll( this );
            pLevelSequencePlayer->Stop();
        }

        m_LevelSequencePlayState_CL.bIsPlaying = false;
    }
}

void ULevelSequenceSubsystem::OnLevelSequenceFinished_CL()
{
    UCoopPuzzleGameInstance* pGameInstance = Cast<UCoopPuzzleGameInstance>( GetGameInstance() );
    if( IsValid( pGameInstance ) == false )
        return;

    OnLevelSequenceFinished_ToServer.ExecuteIfBound( m_LevelSequencePlayState_CL.m_iPlayingLevelSequenceUID, pGameInstance->GetLocalPlayerUID_CL() );
}

void ULevelSequenceSubsystem::OnLevelSequenceFinished_DE( int64 iLevelSequenceUID, int64 iPlayerUID )
{
    auto* pLevelSequencePlayStates = mapLevelSequencePlayStates_DE.Find( iLevelSequenceUID );
    if( pLevelSequencePlayStates == nullptr )
        return;

    pLevelSequencePlayStates->setPlayingUsers.Remove( iPlayerUID );
    if( pLevelSequencePlayStates->setPlayingUsers.IsEmpty() == false )
        return;

    StopLevelSequence( iLevelSequenceUID );
}

void ULevelSequenceSubsystem::LoadLevelSequenceDataIfNotLoaded()
{
    if( m_mapCachedLevelSequenceData.Num() > 0 )
        return;

    if( IsValid( GetGameInstance() ) == false )
        return;

    UDataTableSubsystem* pDataTableSubsystem = GetGameInstance()->GetSubsystem<UDataTableSubsystem>();
    checkf( IsValid( pDataTableSubsystem ) == true, TEXT( "DataTableSubsystem is not valid. Please check class." ) );

    const UDataTable* pLevelSequenceDataTable = pDataTableSubsystem->GetDataTable( EDataTableType::LevelSequence );
    checkf( IsValid( pLevelSequenceDataTable ) == true, TEXT( "pLevelSequenceDataTable is not valid." ) );

    for( const auto& RowPair : pLevelSequenceDataTable->GetRowMap() )
    {
        m_mapCachedLevelSequenceData.Add( RowPair.Key, reinterpret_cast< FLevelSequenceDataRow* >( RowPair.Value ) );
    }
}
