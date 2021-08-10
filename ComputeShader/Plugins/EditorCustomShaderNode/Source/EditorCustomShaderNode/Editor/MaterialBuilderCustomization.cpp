// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialBuilderCustomization.h"

#if WITH_EDITOR

#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"
#include "Editor/PropertyEditor/Public/DetailWidgetRow.h"
#include "RunTime/Slate/Public/Widgets/Text/STextBlock.h"
#include "RunTime/Slate/Public/Widgets/Input/SButton.h"

#include "../Raymarching/RaymarchMaterialBuilder.h"

#define LOCTEXT_NAMESPACE "FMaterialBuilderCustomizationDetails"

TSharedRef<IDetailCustomization> FMaterialBuilderCustomization::MakeInstance()
{
	return MakeShareable(new FMaterialBuilderCustomization);
}

void FMaterialBuilderCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	if (Objects.Num() != 1)
	{
		//Skip customization if select more than one objects
		return;
	}
	ARaymarchMaterialBuilder* MaterialBuilder = (ARaymarchMaterialBuilder*)Objects[0].Get();

	//Hide original property
	DetailBuilder.HideProperty(DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(ARaymarchMaterialBuilder, bShowPreview)));

	//Add custom widget to "Shapes" category
	IDetailCategoryBuilder& OptionsCategory = DetailBuilder.EditCategory("Shapes", FText::FromString(""), ECategoryPriority::Important);
	OptionsCategory.AddCustomRow(FText::FromString("Shapes"))
		.WholeRowContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock).Text(FText::FromString("Display editor preview: "))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(SCheckBox)
				.Style(FEditorStyle::Get(), "RadioButton")
				.IsChecked(this, &FMaterialBuilderCustomization::IsEditorDebugDetailsChecked, MaterialBuilder)
				.OnCheckStateChanged(this, &FMaterialBuilderCustomization::OnEditorDebugDetailsCheckedChanged, MaterialBuilder)
			]
		];
}

ECheckBoxState FMaterialBuilderCustomization::IsEditorDebugDetailsChecked(ARaymarchMaterialBuilder* actor) const
{
	bool bFlag = false;
	if (actor)
	{
		bFlag = actor->bShowPreview;
	}
	return bFlag ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}
void FMaterialBuilderCustomization::OnEditorDebugDetailsCheckedChanged(ECheckBoxState CheckType, ARaymarchMaterialBuilder* actor)
{
	bool bFlag = (CheckType == ECheckBoxState::Checked);
	if (actor)
	{
		actor->Modify();
		actor->bShowPreview = bFlag;
	}
}

#undef LOCTEXT_NAMESPACE
#endif