// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EditorCustomShaderNode/Shader/ShaderFileData.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeShaderFileData() {}
// Cross Module References
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UShaderFileData_NoRegister();
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UShaderFileData();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_EditorCustomShaderNode();
// End Cross Module References
	void UShaderFileData::StaticRegisterNativesUShaderFileData()
	{
	}
	UClass* Z_Construct_UClass_UShaderFileData_NoRegister()
	{
		return UShaderFileData::StaticClass();
	}
	struct Z_Construct_UClass_UShaderFileData_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ExampleString_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_ExampleString;
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SourceFilePath_MetaData[];
#endif
		static const UECodeGen_Private::FStrPropertyParams NewProp_SourceFilePath;
#endif // WITH_EDITORONLY_DATA
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_EDITORONLY_DATA
#endif // WITH_EDITORONLY_DATA
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UShaderFileData_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_EditorCustomShaderNode,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UShaderFileData_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "Shader/ShaderFileData.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Shader/ShaderFileData.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UShaderFileData_Statics::NewProp_ExampleString_MetaData[] = {
		{ "Category", "Properties" },
		{ "ModuleRelativePath", "Shader/ShaderFileData.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UShaderFileData_Statics::NewProp_ExampleString = { "ExampleString", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UShaderFileData, ExampleString), METADATA_PARAMS(Z_Construct_UClass_UShaderFileData_Statics::NewProp_ExampleString_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UShaderFileData_Statics::NewProp_ExampleString_MetaData)) };
#if WITH_EDITORONLY_DATA
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UShaderFileData_Statics::NewProp_SourceFilePath_MetaData[] = {
		{ "Category", "SourceAsset" },
		{ "ModuleRelativePath", "Shader/ShaderFileData.h" },
	};
#endif
	const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UShaderFileData_Statics::NewProp_SourceFilePath = { "SourceFilePath", nullptr, (EPropertyFlags)0x0010000800020001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UShaderFileData, SourceFilePath), METADATA_PARAMS(Z_Construct_UClass_UShaderFileData_Statics::NewProp_SourceFilePath_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UShaderFileData_Statics::NewProp_SourceFilePath_MetaData)) };
#endif // WITH_EDITORONLY_DATA
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UShaderFileData_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UShaderFileData_Statics::NewProp_ExampleString,
#if WITH_EDITORONLY_DATA
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UShaderFileData_Statics::NewProp_SourceFilePath,
#endif // WITH_EDITORONLY_DATA
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UShaderFileData_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UShaderFileData>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UShaderFileData_Statics::ClassParams = {
		&UShaderFileData::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UShaderFileData_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UShaderFileData_Statics::PropPointers),
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UShaderFileData_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UShaderFileData_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UShaderFileData()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UECodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UShaderFileData_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UShaderFileData, 2022446503);
	template<> EDITORCUSTOMSHADERNODE_API UClass* StaticClass<UShaderFileData>()
	{
		return UShaderFileData::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UShaderFileData(Z_Construct_UClass_UShaderFileData, &UShaderFileData::StaticClass, TEXT("/Script/EditorCustomShaderNode"), TEXT("UShaderFileData"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UShaderFileData);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
