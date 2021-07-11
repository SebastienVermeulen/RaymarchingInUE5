#include "ReimportShaderFileFactory.h"
#include "ShaderFileData.h"

bool UReimportShaderFileFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
    UShaderFileData* ShaderFileData = Cast<UShaderFileData>(Obj);
    if (ShaderFileData)
    {
        OutFilenames.Add(UAssetImportData::ResolveImportFilename(ShaderFileData->SourceFilePath, ShaderFileData->GetOutermost()));
        return true;
    }
    return false;
}

void UReimportShaderFileFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
    UShaderFileData* ShaderFileData = Cast<UShaderFileData>(Obj);
    if (ShaderFileData && ensure(NewReimportPaths.Num() == 1))
    {
        ShaderFileData->SourceFilePath = UAssetImportData::SanitizeImportFilename(NewReimportPaths[0], ShaderFileData->GetOutermost());
    }
}

EReimportResult::Type UReimportShaderFileFactory::Reimport(UObject* Obj)
{
    UShaderFileData* ShaderFileData = Cast<UShaderFileData>(Obj);
    if (!ShaderFileData)
    {
        return EReimportResult::Failed;
    }

    const FString Filename = UAssetImportData::ResolveImportFilename(ShaderFileData->SourceFilePath, ShaderFileData->GetOutermost());
    FString Extention = FPaths::GetExtension(Filename);
    if (!Extention.Equals(TEXT("usf")) && !Extention.Equals(TEXT("ush")))
    {
        return EReimportResult::Failed;
    }

    CurrentFilename = Filename;
    FString Data;
    if (FFileHelper::LoadFileToString(Data, *CurrentFilename))
    {
        const TCHAR* Ptr = *Data;
        ShaderFileData->Modify();
        ShaderFileData->MarkPackageDirty();

        UShaderFileFactory::MakeExampleDataFromText(ShaderFileData, Ptr, Ptr + Data.Len());

        // save the source file path and timestamp
        ShaderFileData->SourceFilePath = UAssetImportData::SanitizeImportFilename(CurrentFilename, ShaderFileData->GetOutermost());
    }

    return EReimportResult::Succeeded;
}
