#include "RaymarchedLightingData.h"
#include "RaymarchedLightingProperties.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"

void FRaymarchedLightingData::SetupLighting(UMaterial* Material, FRaymarchedLightingProperties properties)
{
	LightOriginParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(LightOriginParam);
	LightOriginParam->ParameterName = TEXT("LightOrigin");
	LightOriginParam->DefaultValue = properties.LightOrigin;
	LightColorParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(LightColorParam);
	LightColorParam->ParameterName = TEXT("LightColor");
	LightColorParam->DefaultValue = properties.LightColor;
	AmbientColorParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(AmbientColorParam);
	AmbientColorParam->ParameterName = TEXT("AmbientColor");
	AmbientColorParam->DefaultValue = properties.AmbientColor;
	FogMultiplierParam = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(FogMultiplierParam);
	FogMultiplierParam->ParameterName = TEXT("FogMultiplier");
	FogMultiplierParam->DefaultValue = properties.FogMultiplier;
	LightStrengthParam = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(LightStrengthParam);
	LightStrengthParam->ParameterName = TEXT("LightStrength");
	LightStrengthParam->DefaultValue = properties.LightStrength;

#if WITH_EDITOR
	float TotalCurrentHeight = 0.0f;

	LightOriginParam->MaterialExpressionEditorX = -1700.0f;
	LightOriginParam->MaterialExpressionEditorY = 300.0f;
	TotalCurrentHeight += LightOriginParam->GetHeight() + 50.0f;
	LightColorParam->MaterialExpressionEditorX = -1700.0f;
	LightColorParam->MaterialExpressionEditorY = 300.0f + TotalCurrentHeight;
	TotalCurrentHeight += LightColorParam->GetHeight() + 50.0f;
	AmbientColorParam->MaterialExpressionEditorX = -1700.0f;
	AmbientColorParam->MaterialExpressionEditorY = 300.0f + TotalCurrentHeight;
	TotalCurrentHeight += AmbientColorParam->GetHeight() + 50.0f;
	FogMultiplierParam->MaterialExpressionEditorX = -1700.0f;
	FogMultiplierParam->MaterialExpressionEditorY = 100.0f;
	TotalCurrentHeight = 0.0f;
	TotalCurrentHeight += FogMultiplierParam->GetHeight();
	LightStrengthParam->MaterialExpressionEditorX = -1700.0f;
	LightStrengthParam->MaterialExpressionEditorY = 100.0f + TotalCurrentHeight;
#endif
}