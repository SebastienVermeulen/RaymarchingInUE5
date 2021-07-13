// Fill out your copyright notice in the Description page of Project Settings.

#include "ShaderFileFactory.h"
#include "ShaderFileData.h"
#include "UObject/UObjectGlobals.h"

UShaderFileFactory::UShaderFileFactory(const FObjectInitializer& ObjectInitializer) 
    : Super(ObjectInitializer)
{
    Formats.Add(TEXT("usf;Shader file"));
    Formats.Add(TEXT("ush;Shader file"));
    SupportedClass = UShaderFileData::StaticClass();
    bCreateNew = false; // turned off for import
    bEditAfterNew = false; // turned off for import
    bEditorImport = true;
    bText = true;
}

//UObject* UShaderFileFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
//{
//    UShaderFileData* NewObjectAsset = NewObject<UShaderFileData>(InParent, Class, Name, Flags | RF_Transactional);
//    return NewObjectAsset;
//}

UObject* UShaderFileFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
    GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

    // if class type or extension doesn't match, return
    if (InClass != UShaderFileData::StaticClass() ||
        FCString::Stricmp(Type, TEXT("usf")) != 0 &&
        FCString::Stricmp(Type, TEXT("ush")) != 0)
        return nullptr;

    UShaderFileData* Data = CastChecked<UShaderFileData>(NewObject<UShaderFileData>(InParent, InName, Flags));
    MakeExampleDataFromText(Data, Buffer, BufferEnd);

    // save the source file path
    Data->SourceFilePath = UAssetImportData::SanitizeImportFilename(CurrentFilename, Data->GetOutermost());

    GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.Broadcast(this, Data);

    return Data;
}

bool UShaderFileFactory::FactoryCanImport(const FString& Filename)
{
    return FPaths::GetExtension(Filename).Equals(TEXT("usf")) || FPaths::GetExtension(Filename).Equals(TEXT("ush"));
}

void UShaderFileFactory::MakeExampleDataFromText(class UShaderFileData* Data, const TCHAR*& Buffer, const TCHAR* BufferEnd)
{
    Data->ExampleString = Buffer;
}