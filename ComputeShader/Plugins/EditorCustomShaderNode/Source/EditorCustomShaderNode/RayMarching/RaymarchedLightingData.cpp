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
}