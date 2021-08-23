#include "RaymarchedShapeProperties.h"
#include "../../CustomExpression/CustomFileMaterialExpression.h"
#include "../RaymarchedPhysicsShape.h"
#include "../RaymarchedLightingData.h"
#include "../RaymarchMaterialBuilder.h"
#include "DrawDebugHelpers.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Materials/MaterialExpressionCameraPositionWS.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"

UMaterialExpressionNormalize* URaymarchedShapeProperties::RayDir = nullptr;
UMaterialExpressionCameraPositionWS* URaymarchedShapeProperties::RayOrig = nullptr;

URaymarchedShapeProperties::URaymarchedShapeProperties()
	:ShapeType{}
	, StartPosition{}
	, StartRotation{}
	, DiffuseColor{ FColor(0,0,0,1.0f) }
	, SpecularColor{ FColor(0,0,0,1.0f) }
	, Shinyness{1.0f}
	, ExpressionMarch{ nullptr }
	, ExpressionShading{ }
	, ExpressionLighting{ nullptr }
#if WITH_EDITOR
	, TotalEditorHeight{1500}
#endif
	, ObjectOriginParam{ nullptr }
	, ObjectRotationParam{ nullptr }
	, ObjectRadiusParam{ nullptr }
	, DiffuseColorParam{ nullptr }
	, SpecularColorParam{ nullptr }
	, ShinynessParam{ nullptr }
{	
}
