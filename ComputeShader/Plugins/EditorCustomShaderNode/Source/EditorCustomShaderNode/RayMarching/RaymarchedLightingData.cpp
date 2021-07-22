#include "RaymarchedLightingData.h"
#include "RaymarchedLightingProperties.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"

void FRaymarchedLightingData::SetupLighting(UMaterial* Material, FRaymarchedLightingProperties properties)
{
	LightOrigin = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(LightOrigin);
	LightOrigin->ParameterName = TEXT("LightOrigin");
	LightOrigin->DefaultValue = properties.LightOrigin;
	LightColor = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(LightColor);
	LightColor->ParameterName = TEXT("LightColor");
	LightColor->DefaultValue = properties.LightColor;
	AmbientColor = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(AmbientColor);
	AmbientColor->ParameterName = TEXT("AmbientColor");
	AmbientColor->DefaultValue = properties.AmbientColor;
	FogMultiplier = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(FogMultiplier);
	FogMultiplier->ParameterName = TEXT("FogMultiplier");
	FogMultiplier->DefaultValue = properties.FogMultiplier;

#if WITH_EDITOR
	float TotalCurrentHeight = 0.0f;

	LightOrigin->MaterialExpressionEditorX = -1700.0f;
	LightOrigin->MaterialExpressionEditorY = 300.0f;
	TotalCurrentHeight += LightOrigin->GetHeight() + 50.0f;
	LightColor->MaterialExpressionEditorX = -1700.0f;
	LightColor->MaterialExpressionEditorY = 300.0f + TotalCurrentHeight;
	TotalCurrentHeight += LightColor->GetHeight() + 50.0f;
	AmbientColor->MaterialExpressionEditorX = -1700.0f;
	AmbientColor->MaterialExpressionEditorY = 300.0f + TotalCurrentHeight;
	TotalCurrentHeight += AmbientColor->GetHeight() + 50.0f;
	FogMultiplier->MaterialExpressionEditorX = -1700.0f;
	FogMultiplier->MaterialExpressionEditorY = 100.0f;
#endif
}