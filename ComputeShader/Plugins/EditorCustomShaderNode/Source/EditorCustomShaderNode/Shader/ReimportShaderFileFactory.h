#pragma once

#include "CoreMinimal.h"
#include "ShaderFileFactory.h"
#include "EditorReimportHandler.h"
#include "ReimportShaderFileFactory.generated.h"

UCLASS()
class EDITORCUSTOMSHADERNODE_API UReimportShaderFileFactory : public UShaderFileFactory, public FReimportHandler
{
	GENERATED_BODY()
	
    // Begin FReimportHandler interface
    virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
    virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
    virtual EReimportResult::Type Reimport(UObject* Obj) override;
};
