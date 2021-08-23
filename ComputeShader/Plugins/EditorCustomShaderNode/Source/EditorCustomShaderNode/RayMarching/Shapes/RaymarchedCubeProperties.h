#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../ShapeShaderProperties.h"
#include "RaymarchedShapeProperties.h"
#include "RaymarchedShapeInterface.h"
#include "RaymarchedCubeProperties.generated.h"

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

UCLASS(DefaultToInstanced, EditInlineNew)
class URaymarchedCubeProperties : public URaymarchedShapeProperties, public IRaymarchedShapeInterface
{
	GENERATED_BODY()
	
#pragma region Functions
public:	
	URaymarchedCubeProperties();

	virtual ARaymarchedPhysicsShape* CreateShape(ARaymarchMaterialBuilder* Builder, UMaterial* Material, 
		const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx, const int nrShapes) override;
	virtual void UpdateShape(UMaterialInstanceDynamic* Material, const ARaymarchedPhysicsShape* shape, const int idx) override;

	virtual void HookUpOtherShading(UCustomFileMaterialExpression* Shading) override;

#if WITH_EDITOR
	virtual void AdjustEditorHeight(const int nrOfShapes, const float baseheight = 1200.0f, 
		const float shadowShaderHeight = 300.0f) override;
	virtual void DebugDrawShape(UWorld* World) override;
#endif

protected:

private:
	virtual void CreateParameters(UMaterial* Material, const int idx) override;

	virtual void HookupMarching(UMaterial* Material, const FShapeShaderProperties shape, const int idx) override;
	virtual void HookupShading(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, 
		const int idx, const int nrShapes) override;
	virtual void HookupLighting(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, 
		const int idx, const int nrShapes) override;

#pragma endregion

#pragma region Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;

protected:

private:

#pragma endregion
};
