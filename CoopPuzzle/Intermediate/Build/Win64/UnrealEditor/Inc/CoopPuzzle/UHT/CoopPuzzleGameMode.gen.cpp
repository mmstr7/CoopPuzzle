// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CoopPuzzle/CoopPuzzleGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCoopPuzzleGameMode() {}

// Begin Cross Module References
COOPPUZZLE_API UClass* Z_Construct_UClass_ACoopPuzzleGameMode();
COOPPUZZLE_API UClass* Z_Construct_UClass_ACoopPuzzleGameMode_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UPackage* Z_Construct_UPackage__Script_CoopPuzzle();
// End Cross Module References

// Begin Class ACoopPuzzleGameMode
void ACoopPuzzleGameMode::StaticRegisterNativesACoopPuzzleGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ACoopPuzzleGameMode);
UClass* Z_Construct_UClass_ACoopPuzzleGameMode_NoRegister()
{
	return ACoopPuzzleGameMode::StaticClass();
}
struct Z_Construct_UClass_ACoopPuzzleGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "CoopPuzzleGameMode.h" },
		{ "ModuleRelativePath", "CoopPuzzleGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACoopPuzzleGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_ACoopPuzzleGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_CoopPuzzle,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACoopPuzzleGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ACoopPuzzleGameMode_Statics::ClassParams = {
	&ACoopPuzzleGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x008802ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACoopPuzzleGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_ACoopPuzzleGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ACoopPuzzleGameMode()
{
	if (!Z_Registration_Info_UClass_ACoopPuzzleGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ACoopPuzzleGameMode.OuterSingleton, Z_Construct_UClass_ACoopPuzzleGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ACoopPuzzleGameMode.OuterSingleton;
}
template<> COOPPUZZLE_API UClass* StaticClass<ACoopPuzzleGameMode>()
{
	return ACoopPuzzleGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ACoopPuzzleGameMode);
ACoopPuzzleGameMode::~ACoopPuzzleGameMode() {}
// End Class ACoopPuzzleGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_UE5Project_CoopPuzzle_CoopPuzzle_Source_CoopPuzzle_CoopPuzzleGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ACoopPuzzleGameMode, ACoopPuzzleGameMode::StaticClass, TEXT("ACoopPuzzleGameMode"), &Z_Registration_Info_UClass_ACoopPuzzleGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ACoopPuzzleGameMode), 2439103527U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE5Project_CoopPuzzle_CoopPuzzle_Source_CoopPuzzle_CoopPuzzleGameMode_h_3206061896(TEXT("/Script/CoopPuzzle"),
	Z_CompiledInDeferFile_FID_UE5Project_CoopPuzzle_CoopPuzzle_Source_CoopPuzzle_CoopPuzzleGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE5Project_CoopPuzzle_CoopPuzzle_Source_CoopPuzzle_CoopPuzzleGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
