//Credit for info: https://www.youtube.com/watch?v=gMH-_yzof0c

#pragma once

#if WITH_EDITOR

#include "Editor/DetailCustomizations/Public/DetailCustomizations.h"
#include "Editor/PropertyEditor/Public/IDetailCustomization.h"

class ARaymarchMaterialBuilder;

class FMaterialBuilderCustomization: public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	ECheckBoxState IsEditorDebugDetailsChecked(ARaymarchMaterialBuilder* actor) const;
	void OnEditorDebugDetailsCheckedChanged(ECheckBoxState CheckType, ARaymarchMaterialBuilder* actor);
};

#endif