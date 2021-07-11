// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EditorCustomShaderNode/Shader/ShaderFileFactory.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeShaderFileFactory() {}
// Cross Module References
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UShaderFileFactory_NoRegister();
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UShaderFileFactory();
	UNREALED_API UClass* Z_Construct_UClass_UFactory();
	UPackage* Z_Construct_UPackage__Script_EditorCustomShaderNode();
// End Cross Module References
	void UShaderFileFactory::StaticRegisterNativesUShaderFileFactory()
	{
	}
	UClass* Z_Construct_UClass_UShaderFileFactory_NoRegister()
	{
		return UShaderFileFactory::StaticClass();
	}
	struct Z_Construct_UClass_UShaderFileFactory_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UShaderFileFactory_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UFactory,
		(UObject* (*)())Z_Construct_UPackage__Script_EditorCustomShaderNode,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UShaderFileFactory_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "Shader/ShaderFileFactory.h" },
		{ "ModuleRelativePath", "Shader/ShaderFileFactory.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UShaderFileFactory_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UShaderFileFactory>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UShaderFileFactory_Statics::ClassParams = {
		&UShaderFileFactory::StaticClass,
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
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UShaderFileFactory_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UShaderFileFactory_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UShaderFileFactory()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UECodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UShaderFileFactory_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UShaderFileFactory, 2658495919);
	template<> EDITORCUSTOMSHADERNODE_API UClass* StaticClass<UShaderFileFactory>()
	{
		return UShaderFileFactory::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UShaderFileFactory(Z_Construct_UClass_UShaderFileFactory, &UShaderFileFactory::StaticClass, TEXT("/Script/EditorCustomShaderNode"), TEXT("UShaderFileFactory"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UShaderFileFactory);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
