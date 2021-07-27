//Credit for material creation: https://isaratech.com/ue4-programmatically-create-a-new-material-and-inner-nodes/

#include "RaymarchMaterialBuilder.h"
#include "RaymarchedPhysicsShape.h"
#include "RaymarchedLightingProperties.h"
#include "../CustomExpression/CustomFileMaterialExpression.h"

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

ARaymarchMaterialBuilder::ARaymarchMaterialBuilder()
	:RaymarchedShapesProperties{}
	, RaymarchedPhysicsShapes{}
	, Material{}
	, MaterialBaseName{ "M_Material" }
	, PackageName{ "/Game/Materials/" }
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
	Super::Tick(DeltaTime);

	UpdateMaterial();
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

	//Setup shapes
	//Go over all the shape-properties to create the shapes
	for (int i = 0; i < RaymarchedShapesProperties.Num(); i++) 
	{
		FRaymarchedShapeProperties& properties = RaymarchedShapesProperties[i];
		//Find the correct shape
		FShapeShaderProperties* shape = ShapeShaderProperties.FindByPredicate([properties](FShapeShaderProperties x) 
			{ 
				return x.ShapeType == properties.ShapeType; 
			});
		if (shape != nullptr)
		{
			//Create it
			RaymarchedPhysicsShapes.Add(properties.CreateShape(this, Material, *shape, LightingData, i));
		}
		else 
		{
			check(shape);
		}
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
		FRaymarchedShapeProperties& properties = RaymarchedShapesProperties[i];
		properties.UpdateShape(DynamicMaterial, RaymarchedPhysicsShapes[i], i);
	}
}

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

	FRaymarchedShapeProperties temp{};
	temp.RayDir = norm;
	temp.RayOrig = camPos;

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
		UMaterialExpressionComponentMask* maskRGB = NewObject<UMaterialExpressionComponentMask>(Material);
		Material->Expressions.Add(maskRGB);
		maskRGB->R = 1;
		maskRGB->G = 1;
		maskRGB->B = 1;
		maskRGB->A = 0;
		maskRGB->Input.Expression = RaymarchedShapesProperties[0].ExpressionLighting;
		UMaterialExpressionComponentMask* maskA = NewObject<UMaterialExpressionComponentMask>(Material);
		Material->Expressions.Add(maskA);
		maskA->R = 0;
		maskA->G = 0;
		maskA->B = 0;
		maskA->A = 1;
		maskA->Input.Expression = RaymarchedShapesProperties[0].ExpressionLighting;

		UMaterialExpressionMultiply* mult = NewObject<UMaterialExpressionMultiply>(Material);
		Material->Expressions.Add(mult);
		mult->A.Expression = maskRGB;
		mult->B.Expression = RaymarchedShapesProperties[0].ExpressionShading;

		lerp->B.Expression = mult;
		lerp->Alpha.Expression = maskA;

		Material->EmissiveColor.Expression = lerp;

#if WITH_EDITOR
		maskRGB->MaterialExpressionEditorX = 400.0f;
		maskRGB->MaterialExpressionEditorY = RaymarchedShapesProperties[0].ExpressionLighting->MaterialExpressionEditorY;
		maskA->MaterialExpressionEditorX = 400.0f;
		maskA->MaterialExpressionEditorY = maskRGB->MaterialExpressionEditorY + maskRGB->GetHeight();
		mult->MaterialExpressionEditorX = 700.0f;
		mult->MaterialExpressionEditorY = RaymarchedShapesProperties[0].ExpressionShading->MaterialExpressionEditorY;
#endif
	}
	else if (RaymarchedPhysicsShapes.Num() > 1)
	{
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
