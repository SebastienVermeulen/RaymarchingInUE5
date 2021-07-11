// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomFileMatExprCustomization.h"

#if WITH_EDITOR

#include "Editor/PropertyEditor/Public/DetailLayoutBuilder.h"
#include "Editor/PropertyEditor/Public/DetailCategoryBuilder.h"
#include "Editor/PropertyEditor/Public/DetailWidgetRow.h"
#include "RunTime/Slate/Public/Widgets/Text/STextBlock.h"
#include "RunTime/Slate/Public/Widgets/Input/SButton.h"

#include "../CustomExpression/CustomFileMaterialExpression.h"

#define LOCTEXT_NAMESPACE "CustomFileMatExprCustomizationDetails"

TSharedRef<IDetailCustomization> FCustomFileMatExprCustomization::MakeInstance()
{
	return MakeShareable(new FCustomFileMatExprCustomization);
}

void FCustomFileMatExprCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	//Mark which category we are editing
	IDetailCategoryBuilder& CategoryBuilder = DetailBuilder.EditCategory(
		"CustomFileMaterialExpression", FText::GetEmpty(), ECategoryPriority::Important);
	UCustomFileMaterialExpression* Expression = nullptr;
	
	//Go over the objects we are customizing
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	for (TWeakObjectPtr<UObject> customizedObj : CustomizedObjects)
	{
		if (customizedObj.IsValid())
		{
			Expression = Cast<UCustomFileMaterialExpression>(customizedObj);
			if (Expression != nullptr)
			{
				break;
			}
		}
	}

	check(Expression);
}

#undef LOCTEXT_NAMESPACE
#endif