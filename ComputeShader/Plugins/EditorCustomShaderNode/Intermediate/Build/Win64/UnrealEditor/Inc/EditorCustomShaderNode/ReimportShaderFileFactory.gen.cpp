// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EditorCustomShaderNode/Shader/ReimportShaderFileFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeReimportShaderFileFactory() {}
// Cross Module References
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UReimportShaderFileFactory_NoRegister();
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UReimportShaderFileFactory();
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UShaderFileFactory();
	UPackage* Z_Construct_UPackage__Script_EditorCustomShaderNode();
// End Cross Module References
	void UReimportShaderFileFactory::StaticRegisterNativesUReimportShaderFileFactory()
	{
	}
	UClass* Z_Construct_UClass_UReimportShaderFileFactory_NoRegister()
	{
		return UReimportShaderFileFactory::StaticClass();
	}
	struct Z_Construct_UClass_UReimportShaderFileFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UReimportShaderFileFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UShaderFileFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_EditorCustomShaderNode,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UReimportShaderFileFactory_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Shader/ReimportShaderFileFactory.h" },
		{ "ModuleRelativePath", "Shader/ReimportShaderFileFactory.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UReimportShaderFileFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UReimportShaderFileFactory>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UReimportShaderFileFactory_Statics::ClassParams = {
		&UReimportShaderFileFactory::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UReimportShaderFileFactory_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UReimportShaderFileFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UReimportShaderFileFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UECodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UReimportShaderFileFactory_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UReimportShaderFileFactory, 2979283768);
	template<> EDITORCUSTOMSHADERNODE_API UClass* StaticClass<UReimportShaderFileFactory>()
	{
		return UReimportShaderFileFactory::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UReimportShaderFileFactory(Z_Construct_UClass_UReimportShaderFileFactory, &UReimportShaderFileFactory::StaticClass, TEXT("/Script/EditorCustomShaderNode"), TEXT("UReimportShaderFileFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UReimportShaderFileFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
