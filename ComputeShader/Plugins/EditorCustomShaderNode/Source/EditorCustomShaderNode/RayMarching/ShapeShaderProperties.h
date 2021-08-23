#pragma once

#include "CoreMinimal.h"
#include "ShapeShaderProperties.generated.h"

class UShaderFileData;

UENUM()
enum Shape
{
	None     UMETA(DisplayName = "None"),
	Cube     UMETA(DisplayName = "Cube"),
	Sphere   UMETA(DisplayName = "Sphere"),
};

USTRUCT(BlueprintType)
struct FShapeShaderProperties
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
	UShaderFileData* RayMarch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShaderFileData* Lighting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShaderFileData* Shading;

protected:

private:

#pragma endregion
};
