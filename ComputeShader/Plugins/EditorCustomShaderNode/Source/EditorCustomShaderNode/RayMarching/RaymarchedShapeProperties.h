#pragma once

#include "CoreMinimal.h"
#include "ShapeShaderProperties.h"
#include "RaymarchedShapeProperties.generated.h"

class ARaymarchedPhysicsShape;
class UCustomFileMaterialExpression;
class UMaterialInstanceDynamic;
class ARaymarchMaterialBuilder;

class UMaterialExpressionVectorParameter;
class UMaterialExpressionScalarParameter;
class UMaterialExpressionNormalize;
class UMaterialExpressionCameraPositionWS;
class UMaterialExpressionLinearInterpolate;

struct FRaymarchedLightingData;

USTRUCT(BlueprintType)
struct FRaymarchedShapeProperties
{
	GENERATED_USTRUCT_BODY()
	
#pragma region Functions
public:	
	FRaymarchedShapeProperties();

	ARaymarchedPhysicsShape* CreateShape(ARaymarchMaterialBuilder* Builder, UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx, const int nrShapes);
	void UpdateShape(UMaterialInstanceDynamic* Material, const ARaymarchedPhysicsShape* shape, const int idx);

	void HookUpOtherShading(UCustomFileMaterialExpression* Shading);

#if WITH_EDITOR
	void AdjustEditorHeight(const int nrOfShapes, const float baseheight = 1200.0f, const float shadowShaderHeight = 300.0f);
#endif

protected:

private:
	void CreateParameters(UMaterial* Material, const int idx);

	void HookupMarching(UMaterial* Material, const FShapeShaderProperties shape, const int idx);
	void HookupShading(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx, const int nrShapes);
	void HookupLighting(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx, const int nrShapes);

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
	TArray<UCustomFileMaterialExpression*> ExpressionShading;
	UCustomFileMaterialExpression* ExpressionLighting;

	UMaterialExpressionLinearInterpolate* FinalColorExpression;

protected:

private:
#if WITH_EDITOR
	float TotalEditorHeight;
#endif

	UMaterialExpressionVectorParameter* ObjectOriginParam;
	UMaterialExpressionVectorParameter* ObjectRotationParam;
	UMaterialExpressionScalarParameter* ObjectRadiusParam;

	UMaterialExpressionVectorParameter* DiffuseColorParam;
	UMaterialExpressionVectorParameter* SpecularColorParam;
	UMaterialExpressionScalarParameter* ShinynessParam;
#pragma endregion
};
