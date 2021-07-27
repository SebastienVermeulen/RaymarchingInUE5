#pragma once

#include "CoreMinimal.h"
#include "RaymarchedLightingData.generated.h"

class UMaterialExpressionVectorParameter;
class UMaterialExpressionScalarParameter;
struct FRaymarchedLightingProperties;

USTRUCT()
struct FRaymarchedLightingData
{
	GENERATED_USTRUCT_BODY()
	
#pragma region Functions
public:	
	void SetupLighting(UMaterial* Material, FRaymarchedLightingProperties properties);

protected:

private:

#pragma endregion

#pragma region Variables
public:
	UMaterialExpressionVectorParameter* LightOrigin;
	UMaterialExpressionVectorParameter* LightColor;
	UMaterialExpressionVectorParameter* AmbientColor;
	UMaterialExpressionScalarParameter* FogMultiplier;
	UMaterialExpressionScalarParameter* LightStrength;

protected:

private:

#pragma endregion
};
