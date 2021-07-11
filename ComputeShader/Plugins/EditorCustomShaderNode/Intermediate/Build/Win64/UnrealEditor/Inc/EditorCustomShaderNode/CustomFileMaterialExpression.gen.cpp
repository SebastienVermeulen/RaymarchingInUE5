// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "EditorCustomShaderNode/CustomExpression/CustomFileMaterialExpression.h"
#include "Serialization/StructuredArchive.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCustomFileMaterialExpression() {}
// Cross Module References
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UCustomFileMaterialExpression_NoRegister();
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UCustomFileMaterialExpression();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialExpressionCustom();
	UPackage* Z_Construct_UPackage__Script_EditorCustomShaderNode();
	EDITORCUSTOMSHADERNODE_API UClass* Z_Construct_UClass_UShaderFileData_NoRegister();
// End Cross Module References
	void UCustomFileMaterialExpression::StaticRegisterNativesUCustomFileMaterialExpression()
	{
	}
	UClass* Z_Construct_UClass_UCustomFileMaterialExpression_NoRegister()
	{
		return UCustomFileMaterialExpression::StaticClass();
	}
	struct Z_Construct_UClass_UCustomFileMaterialExpression_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CodeFile_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_CodeFile;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCustomFileMaterialExpression_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UMaterialExpressionCustom,
		(UObject* (*)())Z_Construct_UPackage__Script_EditorCustomShaderNode,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCustomFileMaterialExpression_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Object Object Object" },
		{ "IncludePath", "CustomExpression/CustomFileMaterialExpression.h" },
		{ "ModuleRelativePath", "CustomExpression/CustomFileMaterialExpression.h" },
		{ "ObjectInitializerConstructorDeclared", "" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCustomFileMaterialExpression_Statics::NewProp_CodeFile_MetaData[] = {
		{ "Category", "CustomFileMaterialExpression" },
		{ "Comment", "//Shader\n" },
		{ "ModuleRelativePath", "CustomExpression/CustomFileMaterialExpression.h" },
		{ "MultiLine", "TRUE" },
		{ "ToolTip", "Shader" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCustomFileMaterialExpression_Statics::NewProp_CodeFile = { "CodeFile", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(UCustomFileMaterialExpression, CodeFile), Z_Construct_UClass_UShaderFileData_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UCustomFileMaterialExpression_Statics::NewProp_CodeFile_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCustomFileMaterialExpression_Statics::NewProp_CodeFile_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCustomFileMaterialExpression_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCustomFileMaterialExpression_Statics::NewProp_CodeFile,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCustomFileMaterialExpression_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCustomFileMaterialExpression>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCustomFileMaterialExpression_Statics::ClassParams = {
		&UCustomFileMaterialExpression::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UCustomFileMaterialExpression_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UCustomFileMaterialExpression_Statics::PropPointers),
		0,
		0x000820A0u,
		METADATA_PARAMS(Z_Construct_UClass_UCustomFileMaterialExpression_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCustomFileMaterialExpression_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCustomFileMaterialExpression()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UECodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UCustomFileMaterialExpression_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UCustomFileMaterialExpression, 3072448757);
	template<> EDITORCUSTOMSHADERNODE_API UClass* StaticClass<UCustomFileMaterialExpression>()
	{
		return UCustomFileMaterialExpression::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UCustomFileMaterialExpression(Z_Construct_UClass_UCustomFileMaterialExpression, &UCustomFileMaterialExpression::StaticClass, TEXT("/Script/EditorCustomShaderNode"), TEXT("UCustomFileMaterialExpression"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCustomFileMaterialExpression);
	IMPLEMENT_FARCHIVE_SERIALIZER(UCustomFileMaterialExpression)
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
