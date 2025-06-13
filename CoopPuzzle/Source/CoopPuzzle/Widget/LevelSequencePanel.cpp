// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Widget/LevelSequencePanel.h"
#include "CoopPuzzle/Subsystem/WidgetDelegateSubsystem.h"
#include "CoopPuzzle/Data/CoopPuzzleEnums.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CoopPuzzle/Subsystem/LevelSequenceSubsystem.h"

void ULevelSequencePanel::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnLevelSequenceStateChanged.AddUObject( this, &ULevelSequencePanel::UpdateCurrentState );
	}

	if( IsValid( SkipBtn ) == true )
	{
		SkipBtn->OnPressed.AddUniqueDynamic( this, &ULevelSequencePanel::OnPressedSkipBtn );
	}

	UpdateCurrentState( EProcessState::Finished );
}

void ULevelSequencePanel::NativeDestruct()
{
	Super::NativeDestruct();

	UWidgetDelegateSubsystem* pWidgetDelegateSubsystem = IsValid( GetGameInstance() ) == true ? GetGameInstance()->GetSubsystem<UWidgetDelegateSubsystem>() : nullptr;
	if( IsValid( pWidgetDelegateSubsystem ) == true )
	{
		pWidgetDelegateSubsystem->OnLevelSequenceStateChanged.RemoveAll( this );
	}
}

void ULevelSequencePanel::OnPressedSkipBtn()
{
	if( m_eCurrentLevelSequenceState != EProcessState::Playing )
		return;

	ULevelSequenceSubsystem* pLevelSequenceSubsystem = GetGameInstance()->GetSubsystem<ULevelSequenceSubsystem>();
	if( IsValid( pLevelSequenceSubsystem ) == false )
		return;

	pLevelSequenceSubsystem->SkipLevelSequence_CL();

	if( IsValid( SkipBtn ) == false )
		return;

	SkipBtn->SetIsEnabled( false );
}

void ULevelSequencePanel::UpdateCurrentState( EProcessState eState )
{
	if( m_eCurrentLevelSequenceState == eState )
		return;

	m_eCurrentLevelSequenceState = eState;
	switch( m_eCurrentLevelSequenceState )
	{
	case EProcessState::Playing:
	case EProcessState::Waiting:
	{
		SetVisibility( ESlateVisibility::SelfHitTestInvisible );

		if( IsValid( SkipBtn ) == true )
		{
			SkipBtn->SetVisibility( m_eCurrentLevelSequenceState == EProcessState::Playing ? ESlateVisibility::Visible : ESlateVisibility::Collapsed );
			SkipBtn->SetIsEnabled( true );
		}

		if( IsValid( WaitingText ) == true )
		{
			WaitingText->SetVisibility( m_eCurrentLevelSequenceState == EProcessState::Waiting ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed );
		}
	} break;
	case EProcessState::Finished:
	{
		SetVisibility( ESlateVisibility::Collapsed );
	} break;
	default:
		// TODO: checkf Ãâ·Â
		break;
	}
}
