// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShaderFileData.generated.h"

UCLASS(Blueprintable)
class UShaderFileData : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Properties")
        FString ExampleString;

#if WITH_EDITORONLY_DATA
    UPROPERTY(Category = SourceAsset, VisibleAnywhere)
        FString SourceFilePath;
#endif
};
