#pragma once

#include "CoreMinimal.h"
#include "../ShapeShaderProperties.h"
#include "../../CustomExpression/CustomFileMaterialExpression.h"
#include "../RaymarchedPhysicsShape.h"
#include "../RaymarchedLightingData.h"
#include "../RaymarchMaterialBuilder.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialExpressionCameraPositionWS.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "RaymarchedShapeInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class URaymarchedShapeInterface : public UInterface
{
	GENERATED_BODY()
};

class IRaymarchedShapeInterface
{
	GENERATED_BODY()

#pragma region Functions
public:	
	virtual ARaymarchedPhysicsShape* CreateShape(ARaymarchMaterialBuilder* Builder, UMaterial* Material,
		const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx, const int nrShapes)
		PURE_VIRTUAL(IRaymarchedShapeInterface::CreateShape, return nullptr;);
	virtual void UpdateShape(UMaterialInstanceDynamic* Material, const ARaymarchedPhysicsShape* shape, const int idx)
		PURE_VIRTUAL(IRaymarchedShapeInterface::UpdateShape,);

	virtual void HookUpOtherShading(UCustomFileMaterialExpression* Shading) PURE_VIRTUAL(IRaymarchedShapeInterface::HookUpOtherShading,);

#if WITH_EDITOR
	virtual void AdjustEditorHeight(const int nrOfShapes, const float baseheight = 1200.0f, const float shadowShaderHeight = 300.0f)
		PURE_VIRTUAL(IRaymarchedShapeInterface::AdjustEditorHeight, );
	virtual void DebugDrawShape(UWorld* World) PURE_VIRTUAL(IRaymarchedShapeInterface::DebugDrawShape, );
#endif

protected:

private:
	virtual void CreateParameters(UMaterial* Material, const int idx) PURE_VIRTUAL(IRaymarchedShapeInterface::CreateParameters, );

	virtual void HookupMarching(UMaterial* Material, const FShapeShaderProperties shape, const int idx)
		PURE_VIRTUAL(IRaymarchedShapeInterface::HookupMarching, );
	virtual void HookupShading(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, 
		const int idx, const int nrShapes) PURE_VIRTUAL(IRaymarchedShapeInterface::HookupShading, );
	virtual void HookupLighting(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, 
		const int idx, const int nrShapes) PURE_VIRTUAL(IRaymarchedShapeInterface::HookupLighting, );

#pragma endregion
};
