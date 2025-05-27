// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Object/EventTriggerObjectBase.h"
#include "CoopPuzzle/Player/CoopPuzzleCharacter.h"
#include "Components/BoxComponent.h"


AEventTriggerObjectBase::AEventTriggerObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>( TEXT( "TriggerVolume" ) );
}

void AEventTriggerObjectBase::BeginPlay()
{
	Super::BeginPlay();
	

}
