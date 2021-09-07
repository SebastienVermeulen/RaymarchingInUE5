//Credit for material creation: https://isaratech.com/ue4-programmatically-create-a-new-material-and-inner-nodes/

#include "RaymarchMaterialBuilder.h"
#include "PhysicsShapes/RaymarchedPhysicsShape.h"
#include "RaymarchedLightingProperties.h"
#include "../CustomExpression/CustomFileMaterialExpression.h"
#include "Shapes/RaymarchedShapeInterface.h"
#include "DrawDebugHelpers.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Factories/MaterialFactoryNew.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"

#include "Templates/SharedPointer.h" 
#include "Components/PostProcessComponent.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInstanceConstant.h"

#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionCameraPositionWS.h"
#include "Materials/MaterialExpressionWorldPosition.h"
#include "Materials/MaterialExpressionSubtract.h"
#include "Materials/MaterialExpressionSceneTexture.h"
#include "Materials/MaterialExpressionComponentMask.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"
#include "Materials/MaterialExpressionMultiply.h"
#include "Materials/MaterialExpressionAppendVector.h"
#include "Materials/MaterialExpressionCeil.h"
#include "Materials/MaterialExpressionSaturate.h"
#include "Materials/MaterialExpressionMin.h"

ARaymarchMaterialBuilder::ARaymarchMaterialBuilder()
#if WITH_EDITOR
	:bShowPreview{ false }
#endif
	, RaymarchedShapesProperties{}
	, RaymarchedPhysicsShapes{}
	, MaterialBaseName{ "M_Material" }
	, MaterialInstanceBaseName{ "MI_Material" }
	, PackageName{ "/Game/Materials/" }
	, Material{}
{
	PrimaryActorTick.bCanEverTick = true;

	PostProcessing = CreateDefaultSubobject<UPostProcessComponent>(TEXT("Raymarch Volume"));
	PostProcessing->AttachToComponent(RootComponent,
		FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false));

	CreateMaterial();

	UpdateDynamicMaterial();
}
void ARaymarchMaterialBuilder::BeginPlay()
{
	Super::BeginPlay();

	if (Material == nullptr) 
	{
		CreateMaterial();
	}

	PopulateMaterial();
	CreateMaterialInstance();
	UpdateDynamicMaterial();
}
void ARaymarchMaterialBuilder::Tick(float DeltaTime)
{
#if WITH_EDITOR
	if (GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor)
	{
		EditorTick(DeltaTime);
	}
	else
#endif
	{
		Super::Tick(DeltaTime);
		
		UpdateMaterial();
	}
}
void ARaymarchMaterialBuilder::EditorTick(float DeltaTime)
{
	if (bShowPreview)
	{
		DebugDrawShapes();
	}
}

bool ARaymarchMaterialBuilder::ShouldTickIfViewportsOnly() const
{
	if (GetWorld() != nullptr && GetWorld()->WorldType == EWorldType::Editor)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ARaymarchMaterialBuilder::CreateMaterial()
{
	FString tempPackageName = PackageName + MaterialBaseName;
	UPackage* Package = CreatePackage(*tempPackageName);

	// Create an unreal material asset
	UMaterialFactoryNew* MaterialFactory = NewObject<UMaterialFactoryNew>();
	Material = (UMaterial*)MaterialFactory->FactoryCreateNew(
		UMaterial::StaticClass(), 
		Package, 
		*MaterialBaseName, 
		RF_Standalone | RF_Public, 
		NULL, 
		GWarn);

	FAssetRegistryModule::AssetCreated(Material);
	Package->FullyLoad();
	Package->MarkPackageDirty();

	//Set some material settings
	Material->MaterialDomain = EMaterialDomain::MD_PostProcess;
	Material->SetShadingModel(EMaterialShadingModel::MSM_Unlit);
	Material->bCastRayTracedShadows = false;
	Material->BlendableLocation = EBlendableLocation::BL_BeforeTranslucency;

#if WITH_EDITOR
	Material->EditorX = 1500.0f;
	Material->EditorY = -100.0f;
#endif
}
void ARaymarchMaterialBuilder::PopulateMaterial()
{
	//Setup lighting
	LightingData.SetupLighting(Material, LightingProperties);

	//Setup universally used variables
	UMaterialExpressionLinearInterpolate* lerp = SetupStaticVariables();

	URaymarchedShapeProperties* shapeProperties = nullptr;
	IRaymarchedShapeInterface* shapeInterface = nullptr;
	FShapeShaderProperties* shape = nullptr;

	//Setup shapes
	//Go over all the shape-properties to create the shapes, physically and material wise (except for shading, see next loop)
	int nrOfShapes = RaymarchedShapesProperties.Num();
	for (int i = 0; i < nrOfShapes; i++)
	{
		shapeProperties = RaymarchedShapesProperties[i];
		shapeInterface = Cast<IRaymarchedShapeInterface>(shapeProperties);
#if WITH_EDITOR
		shapeInterface->AdjustEditorHeight(nrOfShapes);
#endif
		//Find the correct shape
		shape = ShapeShaderProperties.FindByPredicate([shapeProperties](FShapeShaderProperties x)
			{ 
				return x.ShapeType == shapeProperties->ShapeType;
			});
		if (shape != nullptr)
		{
			//Create it physics
			RaymarchedPhysicsShapes.Add(shapeInterface->CreateShape(this, Material, i));
			
			//Create the material side
			shapeInterface->CreateParameters(Material, i);
			shapeInterface->HookupMarching(Material, *shape, i);
			shapeInterface->HookupLighting(Material, *shape, LightingData, i, nrOfShapes);
		}
		else 
		{
			check(shape);
		}
	}
	//Shading needs to connect later due to it needing nodes of other shapes
	for (int i = 0; i < nrOfShapes; i++)
	{
		shapeProperties = RaymarchedShapesProperties[i];
		shapeInterface = Cast<IRaymarchedShapeInterface>(shapeProperties);
		//Setup shading side
		shapeInterface->HookupShading(Material, RaymarchedShapesProperties, ShapeShaderProperties, LightingData, i, nrOfShapes);
	}

	//Connect the different shapes in the material
	SetupConnectingVariables(lerp);

	UPackage* package = Material->GetPackage();
	package->FullyLoad();
	package->MarkPackageDirty();
	UPackage::SavePackage(package, Material, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *(PackageName + MaterialBaseName + FPackageName::GetAssetPackageExtension()),
		GError, nullptr, true, true, SAVE_NoError);
	
	// Let the material update itself if necessary
	Material->PreEditChange(NULL);
	Material->PostEditChange();
	// make sure that any static meshes, etc using this material will stop using the FMaterialResource of the original
	// material, and will use the new FMaterialResource created when we make a new UMaterial in place
	FGlobalComponentReregisterContext RecreateComponents;
}
void ARaymarchMaterialBuilder::UpdateMaterial()
{
	//Go over all the shapes and update the material accordingly
	//So that visuals follow the physics
	for (int i = 0; i < RaymarchedShapesProperties.Num(); i++)
	{
		Cast<IRaymarchedShapeInterface>(RaymarchedShapesProperties[i])->UpdateShape(DynamicMaterial, RaymarchedPhysicsShapes[i], i);
	}
}
#if WITH_EDITOR
void ARaymarchMaterialBuilder::DebugDrawShapes() 
{
	UWorld* World = GetWorld();
	for (int i = 0; i < RaymarchedShapesProperties.Num(); i++)
	{
		//Check if the shape is a shape and not simply a new/empty object
		if (RaymarchedShapesProperties[i] != nullptr && RaymarchedShapesProperties[i]->ShapeType != Shape::None)
		{
			Cast<IRaymarchedShapeInterface>(RaymarchedShapesProperties[i])->DebugDrawShape(World);
		}
	}
}
#endif

UMaterialExpressionLinearInterpolate* ARaymarchMaterialBuilder::SetupStaticVariables()
{
	UMaterialExpressionWorldPosition* absWP = NewObject<UMaterialExpressionWorldPosition>(Material);
	Material->Expressions.Add(absWP);
	UMaterialExpressionCameraPositionWS* camPos = NewObject<UMaterialExpressionCameraPositionWS>(Material);
	Material->Expressions.Add(camPos);
	UMaterialExpressionSubtract* subtr = NewObject<UMaterialExpressionSubtract>(Material);
	Material->Expressions.Add(subtr);
	UMaterialExpressionNormalize* norm = NewObject<UMaterialExpressionNormalize>(Material);
	Material->Expressions.Add(norm);

	subtr->A.Expression = absWP;
	subtr->B.Expression = camPos;
	norm->VectorInput.Expression = subtr;

	URaymarchedShapeProperties* temp{};
	temp->RayDir = norm;
	temp->RayOrig = camPos;

	//Create class as parent to avoid stupid stuff
	UClass* theClass = FindObject<UClass>(ANY_PACKAGE, TEXT("MaterialExpressionSceneTexture"));
	UMaterialExpression* sceneTex = NewObject<UMaterialExpression>(Material, theClass);
	Material->Expressions.Add(sceneTex);

	//Cast using C style, to avoid any constructors
	UMaterialExpressionSceneTexture* sceneTexCast = (UMaterialExpressionSceneTexture*)(sceneTex);
	//Variables
	sceneTexCast->SceneTextureId = ESceneTextureId::PPI_PostProcessInput0;

	UMaterialExpressionComponentMask* mask = NewObject<UMaterialExpressionComponentMask>(Material);
	Material->Expressions.Add(mask);
	mask->R = 1;
	mask->G = 1;
	mask->B = 1;
	mask->A = 0;
	mask->Input.Expression = sceneTexCast;

	UMaterialExpressionLinearInterpolate* lerp = NewObject<UMaterialExpressionLinearInterpolate>(Material);
	Material->Expressions.Add(lerp);
	lerp->A.Expression = mask;

#if WITH_EDITOR
	absWP->MaterialExpressionEditorX = -1700.0f;
	absWP->MaterialExpressionEditorY = -100.0f;	
	camPos->MaterialExpressionEditorX = -1700.0f;
	camPos->MaterialExpressionEditorY = -100.0f + absWP->GetHeight();
	subtr->MaterialExpressionEditorX = -1450.0f;
	subtr->MaterialExpressionEditorY = -100.0f;
	norm->MaterialExpressionEditorX = -1350.0f;
	norm->MaterialExpressionEditorY = -100.0f;

	sceneTexCast->MaterialExpressionEditorX = 900.0f;
	sceneTexCast->MaterialExpressionEditorY = -100.0f;
	mask->MaterialExpressionEditorX = 1150.0f;
	mask->MaterialExpressionEditorY = -100.0f;
	lerp->MaterialExpressionEditorX = 1300.0f;
	lerp->MaterialExpressionEditorY = -100.0f;
#endif

	return lerp;
}
void ARaymarchMaterialBuilder::SetupConnectingVariables(UMaterialExpressionLinearInterpolate* lerp)
{
	if (RaymarchedShapesProperties.Num() == 1)
	{
		UMaterialExpressionComponentMask* maskA = NewObject<UMaterialExpressionComponentMask>(Material);
		Material->Expressions.Add(maskA);
		maskA->R = 0;
		maskA->G = 0;
		maskA->B = 0;
		maskA->A = 1;
		maskA->Input.Expression = RaymarchedShapesProperties[0]->ExpressionLighting;

		UMaterialExpressionLinearInterpolate* lerpLightingShading = NewObject<UMaterialExpressionLinearInterpolate>(Material);
		Material->Expressions.Add(lerpLightingShading);
		lerpLightingShading->A.Expression = RaymarchedShapesProperties[0]->ExpressionLighting;
		lerpLightingShading->B.Expression = LightingData.AmbientColorParam;
		lerpLightingShading->Alpha.Expression = RaymarchedShapesProperties[0]->ExpressionShading[0];

		UMaterialExpressionComponentMask* maskRGB = NewObject<UMaterialExpressionComponentMask>(Material);
		Material->Expressions.Add(maskRGB);
		maskRGB->R = 1;
		maskRGB->G = 1;
		maskRGB->B = 1;
		maskRGB->A = 0;
		maskRGB->Input.Expression = lerpLightingShading;

		lerp->B.Expression = maskRGB;
		lerp->Alpha.Expression = maskA;

		Material->EmissiveColor.Expression = lerp;


#if WITH_EDITOR
		maskRGB->MaterialExpressionEditorX = 700.0f;
		maskRGB->MaterialExpressionEditorY = RaymarchedShapesProperties[0]->ExpressionLighting->MaterialExpressionEditorY;
		maskA->MaterialExpressionEditorX = 400.0f;
		maskA->MaterialExpressionEditorY = maskRGB->MaterialExpressionEditorY + maskRGB->GetHeight();
		lerpLightingShading->MaterialExpressionEditorX = 400.0f;
		lerpLightingShading->MaterialExpressionEditorY = RaymarchedShapesProperties[0]->ExpressionShading[0]->MaterialExpressionEditorY;
#endif
	}
	else if (RaymarchedPhysicsShapes.Num() > 1)
	{
		//Final masks
		UMaterialExpressionComponentMask* finalMaskA = NewObject<UMaterialExpressionComponentMask>(Material);
		Material->Expressions.Add(finalMaskA);
		finalMaskA->R = 0;
		finalMaskA->G = 0;
		finalMaskA->B = 0;
		finalMaskA->A = 1;
		//maskA->Input.Expression = RaymarchedShapesProperties[0].ExpressionLighting;
		UMaterialExpressionComponentMask* finalMaskRGB = NewObject<UMaterialExpressionComponentMask>(Material);
		Material->Expressions.Add(finalMaskRGB);
		finalMaskRGB->R = 1;
		finalMaskRGB->G = 1;
		finalMaskRGB->B = 1;
		finalMaskRGB->A = 0;
		//maskRGB->Input.Expression = lerpLightingShading;
		lerp->B.Expression = finalMaskRGB;
		lerp->Alpha.Expression = finalMaskA;
		Material->EmissiveColor.Expression = lerp;

		//Multiply individual shading
		for (int i = 0; i < RaymarchedShapesProperties.Num(); i++)
		{
			//Multiply shading
			UMaterialExpressionMultiply* lastShadingMult = nullptr;
			TArray<UCustomFileMaterialExpression*> shading = RaymarchedShapesProperties[i]->ExpressionShading;
			for (int j = 0; j < shading.Num(); j++)
			{
				UMaterialExpressionMultiply* currentMult = NewObject<UMaterialExpressionMultiply>(Material);
				Material->Expressions.Add(currentMult);

				//First connect up the first 2 shading expressions
				if (lastShadingMult == nullptr)
				{
					currentMult->A.Expression = shading[j];
					currentMult->B.Expression = shading[j + 1];
					//Skip one shading expression
					j++;

					lastShadingMult = currentMult;

#if WITH_EDITOR
					currentMult->MaterialExpressionEditorX = 
						RaymarchedShapesProperties[0]->ExpressionShading[0]->MaterialExpressionEditorX +
						RaymarchedShapesProperties[0]->ExpressionShading[0]->GetWidth() + 100;
					currentMult->MaterialExpressionEditorY = 
						RaymarchedShapesProperties[i]->ExpressionShading[0]->MaterialExpressionEditorY;
#endif
				}
				else 
				{
					currentMult->A.Expression = lastShadingMult;
					currentMult->B.Expression = shading[j];

					lastShadingMult = currentMult;

#if WITH_EDITOR
					currentMult->MaterialExpressionEditorX = 
						RaymarchedShapesProperties[0]->ExpressionShading[0]->MaterialExpressionEditorX +
						RaymarchedShapesProperties[0]->ExpressionShading[0]->GetWidth() + 100;
					currentMult->MaterialExpressionEditorY = 
						RaymarchedShapesProperties[i]->ExpressionShading[0]->MaterialExpressionEditorY +
						currentMult->GetHeight() * (j - 1);
#endif
				}
			}

			//Get final lighting color
			UMaterialExpressionComponentMask* lightingMaskA = NewObject<UMaterialExpressionComponentMask>(Material);
			Material->Expressions.Add(lightingMaskA);
			lightingMaskA->R = 0;
			lightingMaskA->G = 0;
			lightingMaskA->B = 0;
			lightingMaskA->A = 1;
			lightingMaskA->Input.Expression = RaymarchedShapesProperties[i]->ExpressionLighting;
			UMaterialExpressionComponentMask* ambientMaskRGB = NewObject<UMaterialExpressionComponentMask>(Material);
			Material->Expressions.Add(ambientMaskRGB);
			ambientMaskRGB->R = 1;
			ambientMaskRGB->G = 1;
			ambientMaskRGB->B = 1;
			ambientMaskRGB->A = 0;
			ambientMaskRGB->Input.Expression = LightingData.AmbientColorParam;

			UMaterialExpressionAppendVector* append = NewObject<UMaterialExpressionAppendVector>(Material);
			Material->Expressions.Add(append);
			append->A.Expression = ambientMaskRGB;
			append->B.Expression = lightingMaskA;

			RaymarchedShapesProperties[i]->FinalColorExpression = NewObject<UMaterialExpressionLinearInterpolate>(Material);
			Material->Expressions.Add(RaymarchedShapesProperties[i]->FinalColorExpression);
			RaymarchedShapesProperties[i]->FinalColorExpression->A.Expression = append;
			RaymarchedShapesProperties[i]->FinalColorExpression->B.Expression = RaymarchedShapesProperties[i]->ExpressionLighting;
			RaymarchedShapesProperties[i]->FinalColorExpression->Alpha.Expression = lastShadingMult;

#if WITH_EDITOR
			lightingMaskA->MaterialExpressionEditorX = lastShadingMult->MaterialExpressionEditorX;
			lightingMaskA->MaterialExpressionEditorY = RaymarchedShapesProperties[i]->ExpressionShading[0]->MaterialExpressionEditorY - lastShadingMult->GetHeight();			
			ambientMaskRGB->MaterialExpressionEditorX = lastShadingMult->MaterialExpressionEditorX;
			ambientMaskRGB->MaterialExpressionEditorY = RaymarchedShapesProperties[i]->ExpressionShading[0]->MaterialExpressionEditorY - lastShadingMult->GetHeight() - lightingMaskA->GetHeight();
						
			append->MaterialExpressionEditorX = lastShadingMult->MaterialExpressionEditorX + lastShadingMult->GetWidth() + 50;
			append->MaterialExpressionEditorY = lightingMaskA->MaterialExpressionEditorY;

			RaymarchedShapesProperties[i]->FinalColorExpression->MaterialExpressionEditorX = 
				lastShadingMult->MaterialExpressionEditorX + lastShadingMult->GetWidth() + 250;
			RaymarchedShapesProperties[i]->FinalColorExpression->MaterialExpressionEditorY =
				RaymarchedShapesProperties[i]->ExpressionShading[0]->MaterialExpressionEditorY;
#endif
		}
		//Connect shading with other parameters
		for (int i = 0; i < RaymarchedShapesProperties.Num(); i++)
		{
			TArray<UCustomFileMaterialExpression*> shading = RaymarchedShapesProperties[i]->ExpressionShading;
			for (int j = 0; j < shading.Num(); j++)
			{			
				if (j == i) 
				{
					continue;
				}
				Cast<IRaymarchedShapeInterface>(RaymarchedShapesProperties[j])->HookUpOtherShading(shading[j]);
			}
		}
		
		FName compareOutputStr = "dist";
		auto outputLambda = [&compareOutputStr](FCustomOutput x) { return x.OutputName == compareOutputStr; };

		//Depth test
		UMaterialExpressionLinearInterpolate* depthLerp = nullptr;
		UMaterialExpressionMin* lastMin = nullptr;
		for (int i = 0; i < RaymarchedShapesProperties.Num(); i++)
		{
			if (depthLerp == nullptr)
			{
				UMaterialExpressionSubtract* sub = NewObject<UMaterialExpressionSubtract>(Material);
				Material->Expressions.Add(sub);
				RaymarchedShapesProperties[i]->ExpressionMarch->ConnectExpression(
					&sub->A, RaymarchedShapesProperties[i]->ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1);
				RaymarchedShapesProperties[i + 1]->ExpressionMarch->ConnectExpression(
					&sub->B, RaymarchedShapesProperties[i + 1]->ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1);
				UMaterialExpressionSaturate* sat = NewObject<UMaterialExpressionSaturate>(Material);
				Material->Expressions.Add(sat);
				sat->Input.Expression = sub;
				UMaterialExpressionCeil* ceil = NewObject<UMaterialExpressionCeil>(Material);
				Material->Expressions.Add(ceil);
				ceil->Input.Expression = sat;
				depthLerp = NewObject<UMaterialExpressionLinearInterpolate>(Material);
				Material->Expressions.Add(depthLerp);
				depthLerp->A.Expression = RaymarchedShapesProperties[i]->FinalColorExpression;
				depthLerp->B.Expression = RaymarchedShapesProperties[i + 1]->FinalColorExpression;
				depthLerp->Alpha.Expression = ceil;
				lastMin = NewObject<UMaterialExpressionMin>(Material);
				Material->Expressions.Add(lastMin);
				RaymarchedShapesProperties[i]->ExpressionMarch->ConnectExpression(
					&lastMin->A, RaymarchedShapesProperties[i]->ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1);
				RaymarchedShapesProperties[i + 1]->ExpressionMarch->ConnectExpression(
					&lastMin->B, RaymarchedShapesProperties[i + 1]->ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1);

#if WITH_EDITOR
				sub->MaterialExpressionEditorX =
					RaymarchedShapesProperties[0]->ExpressionLighting->MaterialExpressionEditorX + 300;
				sub->MaterialExpressionEditorY =
					RaymarchedShapesProperties[i]->ExpressionLighting->MaterialExpressionEditorY -
					RaymarchedShapesProperties[i]->ExpressionLighting->GetHeight();
				sat->MaterialExpressionEditorX = sub->MaterialExpressionEditorX + sub->GetWidth() + 50;
				sat->MaterialExpressionEditorY =
					RaymarchedShapesProperties[i]->ExpressionLighting->MaterialExpressionEditorY -
					RaymarchedShapesProperties[i]->ExpressionLighting->GetHeight();
				ceil->MaterialExpressionEditorX = sat->MaterialExpressionEditorX + sat->GetWidth() + 50;
				ceil->MaterialExpressionEditorY =
					RaymarchedShapesProperties[i]->ExpressionLighting->MaterialExpressionEditorY -
					RaymarchedShapesProperties[i]->ExpressionLighting->GetHeight();
				depthLerp->MaterialExpressionEditorX = ceil->MaterialExpressionEditorX + ceil->GetWidth() + 50;
				depthLerp->MaterialExpressionEditorY =
					RaymarchedShapesProperties[i]->ExpressionLighting->MaterialExpressionEditorY -
					RaymarchedShapesProperties[i]->ExpressionLighting->GetHeight();				
				lastMin->MaterialExpressionEditorX = sub->MaterialExpressionEditorX;
				lastMin->MaterialExpressionEditorY = sub->MaterialExpressionEditorY - sub->GetHeight();
#endif

				i++;
			}
			else 
			{
				UMaterialExpressionSubtract* sub = NewObject<UMaterialExpressionSubtract>(Material);
				Material->Expressions.Add(sub);
				sub->A.Expression = lastMin;
				RaymarchedShapesProperties[i]->ExpressionMarch->ConnectExpression(
					&sub->B, RaymarchedShapesProperties[i]->ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1);
				UMaterialExpressionSaturate* sat = NewObject<UMaterialExpressionSaturate>(Material);
				Material->Expressions.Add(sat);
				sat->Input.Expression = sub;
				UMaterialExpressionCeil* ceil = NewObject<UMaterialExpressionCeil>(Material);
				Material->Expressions.Add(ceil);
				ceil->Input.Expression = sat;
				UMaterialExpressionLinearInterpolate* lastDepthLerp = depthLerp;
				depthLerp = NewObject<UMaterialExpressionLinearInterpolate>(Material);
				Material->Expressions.Add(depthLerp);
				depthLerp->A.Expression = lastDepthLerp;
				depthLerp->B.Expression = RaymarchedShapesProperties[i]->FinalColorExpression;
				depthLerp->Alpha.Expression = ceil;
				UMaterialExpressionMin* lastMinPrevious = lastMin;
				lastMin = NewObject<UMaterialExpressionMin>(Material);
				Material->Expressions.Add(lastMin);
				lastMin->A.Expression = lastMinPrevious;
				RaymarchedShapesProperties[i]->ExpressionMarch->ConnectExpression(
					&lastMin->B, RaymarchedShapesProperties[i]->ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1);

#if WITH_EDITOR
				sub->MaterialExpressionEditorX =
					RaymarchedShapesProperties[0]->ExpressionLighting->MaterialExpressionEditorX + 300;
				sub->MaterialExpressionEditorY =
					RaymarchedShapesProperties[i]->ExpressionLighting->MaterialExpressionEditorY -
					RaymarchedShapesProperties[i]->ExpressionLighting->GetHeight();
				sat->MaterialExpressionEditorX = sub->MaterialExpressionEditorX + sub->GetWidth() + 50;
				sat->MaterialExpressionEditorY =
					RaymarchedShapesProperties[i]->ExpressionLighting->MaterialExpressionEditorY -
					RaymarchedShapesProperties[i]->ExpressionLighting->GetHeight();
				ceil->MaterialExpressionEditorX = sat->MaterialExpressionEditorX + sat->GetWidth() + 50;
				ceil->MaterialExpressionEditorY =
					RaymarchedShapesProperties[i]->ExpressionLighting->MaterialExpressionEditorY -
					RaymarchedShapesProperties[i]->ExpressionLighting->GetHeight();
				depthLerp->MaterialExpressionEditorX = ceil->MaterialExpressionEditorX + ceil->GetWidth() + 50;
				depthLerp->MaterialExpressionEditorY =
					RaymarchedShapesProperties[i]->ExpressionLighting->MaterialExpressionEditorY -
					RaymarchedShapesProperties[i]->ExpressionLighting->GetHeight();
				lastMin->MaterialExpressionEditorX = sub->MaterialExpressionEditorX;
				lastMin->MaterialExpressionEditorY = sub->MaterialExpressionEditorY - sub->GetHeight();
#endif
			}
		}

		finalMaskRGB->Input.Expression = depthLerp;
		finalMaskA->Input.Expression = depthLerp;

#if WITH_EDITOR
		//Final masks
		finalMaskRGB->MaterialExpressionEditorX = lerp->MaterialExpressionEditorX - 150.0f;
		finalMaskRGB->MaterialExpressionEditorY = lerp->MaterialExpressionEditorY + finalMaskRGB->GetHeight();
		finalMaskA->MaterialExpressionEditorX = lerp->MaterialExpressionEditorX - 150.0f;
		finalMaskA->MaterialExpressionEditorY = finalMaskRGB->MaterialExpressionEditorY + finalMaskRGB->GetHeight();
#endif
	}
}

void ARaymarchMaterialBuilder::CreateMaterialInstance()
{
	FString tempPackageName = PackageName + MaterialInstanceBaseName;
	UPackage* Package = CreatePackage(*tempPackageName);

	// Create an unreal material asset
	UMaterialInstanceConstantFactoryNew* MaterialFactory = NewObject<UMaterialInstanceConstantFactoryNew>();
	MaterialInstance = (UMaterialInstanceConstant*)MaterialFactory->FactoryCreateNew(
		UMaterialInstanceConstant::StaticClass(),
		Package,
		*MaterialInstanceBaseName,
		RF_Standalone | RF_Public,
		NULL,
		GWarn);
	MaterialInstance->Parent = Material;

	FAssetRegistryModule::AssetCreated(MaterialInstance);
	Package->FullyLoad();
	Package->MarkPackageDirty();
}
void ARaymarchMaterialBuilder::UpdateDynamicMaterial()
{
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, GetWorld());
	PostProcessing->AddOrUpdateBlendable(DynamicMaterial);
}
