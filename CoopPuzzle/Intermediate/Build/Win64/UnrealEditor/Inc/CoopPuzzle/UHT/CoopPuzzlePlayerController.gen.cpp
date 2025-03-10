// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CoopPuzzle/CoopPuzzlePlayerController.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCoopPuzzlePlayerController() {}

// Begin Cross Module References
COOPPUZZLE_API UClass* Z_Construct_UClass_ACoopPuzzlePlayerController();
COOPPUZZLE_API UClass* Z_Construct_UClass_ACoopPuzzlePlayerController_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
NIAGARA_API UClass* Z_Construct_UClass_UNiagaraSystem_NoRegister();
UPackage* Z_Construct_UPackage__Script_CoopPuzzle();
// End Cross Module References

// Begin Class ACoopPuzzlePlayerController
void ACoopPuzzlePlayerController::StaticRegisterNativesACoopPuzzlePlayerController()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ACoopPuzzlePlayerController);
UClass* Z_Construct_UClass_ACoopPuzzlePlayerController_NoRegister()
{
	return ACoopPuzzlePlayerController::StaticClass();
}
struct Z_Construct_UClass_ACoopPuzzlePlayerController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "CoopPuzzlePlayerController.h" },
		{ "ModuleRelativePath", "CoopPuzzlePlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ShortPressThreshold_MetaData[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Time Threshold to know if it was a short press */" },
#endif
		{ "ModuleRelativePath", "CoopPuzzlePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Time Threshold to know if it was a short press" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FXCursor_MetaData[] = {
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** FX Class that we will spawn when clicking */" },
#endif
		{ "ModuleRelativePath", "CoopPuzzlePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "FX Class that we will spawn when clicking" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultMappingContext_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** MappingContext */" },
#endif
		{ "ModuleRelativePath", "CoopPuzzlePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "MappingContext" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SetDestinationClickAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Jump Input Action */" },
#endif
		{ "ModuleRelativePath", "CoopPuzzlePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Jump Input Action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SetDestinationTouchAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Jump Input Action */" },
#endif
		{ "ModuleRelativePath", "CoopPuzzlePlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Jump Input Action" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_ShortPressThreshold;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FXCursor;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DefaultMappingContext;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SetDestinationClickAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SetDestinationTouchAction;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACoopPuzzlePlayerController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_ShortPressThreshold = { "ShortPressThreshold", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACoopPuzzlePlayerController, ShortPressThreshold), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ShortPressThreshold_MetaData), NewProp_ShortPressThreshold_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_FXCursor = { "FXCursor", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACoopPuzzlePlayerController, FXCursor), Z_Construct_UClass_UNiagaraSystem_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FXCursor_MetaData), NewProp_FXCursor_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_DefaultMappingContext = { "DefaultMappingContext", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACoopPuzzlePlayerController, DefaultMappingContext), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultMappingContext_MetaData), NewProp_DefaultMappingContext_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_SetDestinationClickAction = { "SetDestinationClickAction", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACoopPuzzlePlayerController, SetDestinationClickAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SetDestinationClickAction_MetaData), NewProp_SetDestinationClickAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_SetDestinationTouchAction = { "SetDestinationTouchAction", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ACoopPuzzlePlayerController, SetDestinationTouchAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SetDestinationTouchAction_MetaData), NewProp_SetDestinationTouchAction_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_ShortPressThreshold,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_FXCursor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_DefaultMappingContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_SetDestinationClickAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::NewProp_SetDestinationTouchAction,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_CoopPuzzle,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::ClassParams = {
	&ACoopPuzzlePlayerController::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::PropPointers),
	0,
	0x008002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::Class_MetaDataParams), Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ACoopPuzzlePlayerController()
{
	if (!Z_Registration_Info_UClass_ACoopPuzzlePlayerController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ACoopPuzzlePlayerController.OuterSingleton, Z_Construct_UClass_ACoopPuzzlePlayerController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ACoopPuzzlePlayerController.OuterSingleton;
}
template<> COOPPUZZLE_API UClass* StaticClass<ACoopPuzzlePlayerController>()
{
	return ACoopPuzzlePlayerController::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ACoopPuzzlePlayerController);
ACoopPuzzlePlayerController::~ACoopPuzzlePlayerController() {}
// End Class ACoopPuzzlePlayerController

// Begin Registration
struct Z_CompiledInDeferFile_FID_UE5Project_CoopPuzzle_CoopPuzzle_Source_CoopPuzzle_CoopPuzzlePlayerController_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ACoopPuzzlePlayerController, ACoopPuzzlePlayerController::StaticClass, TEXT("ACoopPuzzlePlayerController"), &Z_Registration_Info_UClass_ACoopPuzzlePlayerController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ACoopPuzzlePlayerController), 2800968044U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UE5Project_CoopPuzzle_CoopPuzzle_Source_CoopPuzzle_CoopPuzzlePlayerController_h_419751680(TEXT("/Script/CoopPuzzle"),
	Z_CompiledInDeferFile_FID_UE5Project_CoopPuzzle_CoopPuzzle_Source_CoopPuzzle_CoopPuzzlePlayerController_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UE5Project_CoopPuzzle_CoopPuzzle_Source_CoopPuzzle_CoopPuzzlePlayerController_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
