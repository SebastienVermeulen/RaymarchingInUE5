#pragma once

#include "CoreMinimal.h"
#include "ShapeShaderProperties.h"
#include "RaymarchedShapeProperties.generated.h"

class ARaymarchedPhysicsShape;
class UCustomFileMaterialExpression;
class UMaterialExpressionVectorParameter;
class UMaterialExpressionScalarParameter;
class UMaterialExpressionNormalize;
class UMaterialExpressionCameraPositionWS;
class UMaterialInstanceDynamic;
class ARaymarchMaterialBuilder;

struct FRaymarchedLightingData;

USTRUCT(BlueprintType)
struct FRaymarchedShapeProperties
{
	GENERATED_USTRUCT_BODY()
	
#pragma region Functions
public:	
	FRaymarchedShapeProperties();

	ARaymarchedPhysicsShape* CreateShape(ARaymarchMaterialBuilder* Builder, UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx);
	void UpdateShape(UMaterialInstanceDynamic* Material, const ARaymarchedPhysicsShape* shape, const int idx);

protected:

private:
	void CreateParameters(UMaterial* Material, const int idx);

	void HookupMarching(UMaterial* Material, const FShapeShaderProperties shape, const int idx);
	void HookupShading(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx);
	void HookupLighting(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor DiffuseColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor SpecularColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Shinyness;

	static UMaterialExpressionNormalize* RayDir;
	static UMaterialExpressionCameraPositionWS* RayOrig;

	UCustomFileMaterialExpression* ExpressionMarch;
	UCustomFileMaterialExpression* ExpressionShading;
	UCustomFileMaterialExpression* ExpressionLighting;

protected:
#if WITH_EDITOR
	float TotalEditorHeight;
#endif

private:
	UMaterialExpressionVectorParameter* ObjectOriginParam;
	UMaterialExpressionVectorParameter* ObjectRotationParam;
	UMaterialExpressionScalarParameter* ObjectRadiusParam;

	UMaterialExpressionVectorParameter* DiffuseColorParam;
	UMaterialExpressionVectorParameter* SpecularColorParam;
	UMaterialExpressionScalarParameter* ShinynessParam;
#pragma endregion
};
