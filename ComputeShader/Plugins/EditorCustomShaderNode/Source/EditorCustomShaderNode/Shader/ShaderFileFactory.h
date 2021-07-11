// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealEd.h"
#include "ShaderFileFactory.generated.h"

/**
 * 
 */
UCLASS()
class UShaderFileFactory : public UFactory
{
    GENERATED_UCLASS_BODY()

    //virtual UObject* FactoryCreateNew(
    //    UClass* Class, 
    //    UObject* InParent, 
    //    FName Name, 
    //    EObjectFlags Flags, 
    //    UObject* Context, 
    //    FFeedbackContext* Warn) override;
    virtual UObject* FactoryCreateText(
        UClass* InClass, 
        UObject* InParent, 
        FName InName, 
        EObjectFlags Flags, 
        UObject* Context, 
        const TCHAR* Type, 
        const TCHAR*& Buffer, 
        const TCHAR* BufferEnd, 
        FFeedbackContext* Warn) override;
    virtual bool FactoryCanImport(const FString& Filename) override;

    // helper function
    static void MakeExampleDataFromText(class UShaderFileData* Data, const TCHAR*& Buffer, const TCHAR* BufferEnd);
};
