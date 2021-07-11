// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AssetTypeActions_Base.h"

class FShaderFileTypeActions : public FAssetTypeActions_Base
{
public:
    FShaderFileTypeActions(EAssetTypeCategories::Type InAssetCategory);

    // IAssetTypeActions interface
    virtual FText GetName() const override;
    virtual FColor GetTypeColor() const override;
    virtual UClass* GetSupportedClass() const override;
    virtual uint32 GetCategories() override;
    // End of IAssetTypeActions interface

    virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
    virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;

    void ExecuteReimport(TArray<TWeakObjectPtr<UShaderFileData>> Objects);

private:
    EAssetTypeCategories::Type MyAssetCategory;
};
