#pragma once

#include "CoreMinimal.h"
#include "ShapeShaderProperties.h"
#include "RaymarchedShapeProperties.generated.h"

USTRUCT(BlueprintType)
struct FRaymarchedShapeProperties
{
	GENERATED_USTRUCT_BODY()
	
#pragma region Functions
public:	

protected:

private:

#pragma endregion

#pragma region Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<Shape> ShapeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator StartRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;

protected:

private:

#pragma endregion
};
