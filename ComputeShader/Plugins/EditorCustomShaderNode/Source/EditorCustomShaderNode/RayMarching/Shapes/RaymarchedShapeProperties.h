#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../ShapeShaderProperties.h"
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

UCLASS(Abstract, DefaultToInstanced, EditInlineNew)
class URaymarchedShapeProperties : public UDataAsset
{
	GENERATED_BODY()
	
#pragma region Functions
public:	
	URaymarchedShapeProperties();

protected:

private:

#pragma endregion

#pragma region Variables
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TEnumAsByte<Shape> ShapeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator StartRotation;

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

private:

};
