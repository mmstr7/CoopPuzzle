// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Object/EventTriggerMovingPlatform.h"
#include "EventTriggerMovingPlatform.h"
#include "Net/UnrealNetwork.h"

AEventTriggerMovingPlatform::AEventTriggerMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	MovingPlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "MovingPlatformMesh" ) );
	MovingPlatformMesh->SetupAttachment( RootComponent );
}

void AEventTriggerMovingPlatform::GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME_CONDITION( AEventTriggerMovingPlatform, R_fMovePercent, COND_InitialOnly );
}

void AEventTriggerMovingPlatform::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

	if( IsValid( MovingPlatformMesh ) == false )
		return;

	if( GetTriggerState() != EEventTriggerState::Triggered )
		return;

	R_fMovePercent += DeltaSeconds * Speed;

	// �պ� ó��
	if( R_fMovePercent > 1.f )
	{
		R_fMovePercent -= 2.f;
	}

	// ��ġ ����
	float fLerpAlpha = FMath::Abs( R_fMovePercent );
	MovingPlatformMesh->SetRelativeLocation( FMath::Lerp( StartRelativeLoc, EndRelativeLoc, fLerpAlpha ) );
}