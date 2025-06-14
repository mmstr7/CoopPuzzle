// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzle/Subsystem/TooltipSubsystem.h"
#include "CoopPuzzle/Widget/TooltipBase.h"
#include "CoopPuzzle/Widget/ItemTooltip.h"

UTooltipBase* UTooltipSubsystem::GetTooltipWidget( ETooltipType eTooltipType, const FName& Key )
{
	UTooltipBase* pTooltip = m_mapCachedTooltip.FindRef( eTooltipType );
	if( IsValid( pTooltip ) == false )
	{
		pTooltip = CreateWidget<UTooltipBase>( GetWorld(), m_mapTooltipClass.FindRef( eTooltipType ) );
		m_mapCachedTooltip.Add( eTooltipType, pTooltip );
	}
	
	if( IsValid( pTooltip ) == false )
		return nullptr;

	// 툴팁 세팅 후 반환
	pTooltip->SetupTooltipWidget( Key );
	return pTooltip;
}

void UTooltipSubsystem::Initialize( FSubsystemCollectionBase& Collection )
{
	Super::Initialize( Collection );

	for( uint8 i = 0; i < static_cast< uint8 >( ETooltipType::MAX ); ++i )
	{
		ETooltipType eTooltipType = static_cast< ETooltipType >( i );
		TSubclassOf<UTooltipBase> TooltipClass;

		// 툴팁 타입이 추가 될 때 마다 여기에 클래스 세팅을 합니다.
		switch( eTooltipType )
		{
		case ETooltipType::Item:
		{
			TooltipClass = StaticLoadClass( UTooltipBase::StaticClass(), nullptr, TEXT( "/Game/TopDown/Blueprints/Widget/WBP_ItemTooltip.WBP_ItemTooltip_C" ) );
		} break;
		}

		checkf( IsValid( TooltipClass ) == true, TEXT( "Tooltip class for [%s] is not registered." ), *UEnum::GetValueAsString( eTooltipType ) );

		m_mapTooltipClass.Add( eTooltipType, TooltipClass );
	}
}
