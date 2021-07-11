#include "ShaderFileTypeActions.h"
#include "ShaderFileData.h"
#include "EditorReimportHandler.h"

FShaderFileTypeActions::FShaderFileTypeActions(EAssetTypeCategories::Type InAssetCategory)
    : MyAssetCategory(InAssetCategory)
{
}

FText FShaderFileTypeActions::GetName() const
{
    return FText::FromString("Shader File");
}

FColor FShaderFileTypeActions::GetTypeColor() const
{
    return FColor(230, 205, 165);
}

UClass* FShaderFileTypeActions::GetSupportedClass() const
{
    return UShaderFileData::StaticClass();
}

uint32 FShaderFileTypeActions::GetCategories()
{
    return MyAssetCategory;
}

void FShaderFileTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) 
{
    auto ExampleDataImports = GetTypedWeakObjectPtrs<UShaderFileData>(InObjects);

    MenuBuilder.AddMenuEntry(
        FText::FromString("Reimport"),
        FText::FromString("Reimports shader data."),
        FSlateIcon(),
        FUIAction(
            FExecuteAction::CreateSP(this, &FShaderFileTypeActions::ExecuteReimport, ExampleDataImports),
            FCanExecuteAction()
        )
    );
}
void FShaderFileTypeActions::ExecuteReimport(TArray<TWeakObjectPtr<UShaderFileData>> Objects)
{
    for (auto ObjIt = Objects.CreateConstIterator(); ObjIt; ++ObjIt)
    {
        auto Object = (*ObjIt).Get();
        if (Object)
        {
            FReimportManager::Instance()->Reimport(Object, /*bAskForNewFileIfMissing=*/true);
        }
    }
}
