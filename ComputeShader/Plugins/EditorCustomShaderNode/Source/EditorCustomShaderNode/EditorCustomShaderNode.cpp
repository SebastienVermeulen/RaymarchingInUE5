// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorCustomShaderNode.h"

#define LOCTEXT_NAMESPACE "FEditorCustomShaderNodeModule"

#include "Interfaces/IPluginManager.h"
#include "Shader/ShaderFileTypeActions.h"
#include "IAssetTypeActions.h"

#if WITH_EDITOR

#include "Editor/PropertyEditor/Public/PropertyEditorModule.h"
#include "Editor/CustomFileMatExprCustomization.h"

#endif

void FEditorCustomShaderNodeModule::StartupModule()
{
    //Shader include paths
    FString shaderProjectDir = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Shaders"));
    AddShaderSourceDirectoryMapping("/Project", shaderProjectDir);

    //Register custom factory types
    IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
    // add custom category
    EAssetTypeCategories::Type ExampleCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Shaders")), FText::FromString("Shaders"));
    // register our custom asset with example category
    TSharedPtr<IAssetTypeActions> Action = MakeShareable(new FShaderFileTypeActions(ExampleCategory));
    AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
    // saved it here for unregister later
    CreatedAssetTypeActions.Add(Action);

#if WITH_EDITOR
    //Add custom editor stuff to the CustomFileMaterialExpression class 
    FPropertyEditorModule& EditorModule =
        FModuleManager::LoadModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));
    EditorModule.RegisterCustomClassLayout("CustomFileMaterialExpression",
        FOnGetDetailCustomizationInstance::CreateStatic(&FCustomFileMatExprCustomization::MakeInstance));
#endif

    IModuleInterface::StartupModule();
}

void FEditorCustomShaderNodeModule::ShutdownModule()
{
    // Unregister all the asset types that we registered
    if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
    {
        IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
        for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
        {
            AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
        }
    }
    CreatedAssetTypeActions.Empty();

    IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorCustomShaderNodeModule, EditorCustomShaderNode)