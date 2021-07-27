#pragma once

#include "CoreMinimal.h"
#include "RaymarchedLightingProperties.generated.h"

USTRUCT(BlueprintType)
struct FRaymarchedLightingProperties
{
	GENERATED_USTRUCT_BODY()
	
#pragma region Functions
public:	
	FRaymarchedLightingProperties();

protected:

private:

#pragma endregion

#pragma region Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LightOrigin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor LightColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor AmbientColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FogMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightStrength;

protected:

private:

#pragma endregion
};
