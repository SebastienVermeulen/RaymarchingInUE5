//Credit for material creation: https://isaratech.com/ue4-programmatically-create-a-new-material-and-inner-nodes/

#include "RaymarchMaterialBuilder.h"
#include "RaymarchedPhysicsShape.h"
#include "Factories/MaterialFactoryNew.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Templates/SharedPointer.h"
#include "RaymarchedLightingProperties.h"

#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionCameraPositionWS.h"
#include "Materials/MaterialExpressionWorldPosition.h"
#include "Materials/MaterialExpressionSubtract.h"
#include "Materials/MaterialExpressionSceneTexture.h"
#include "Materials/MaterialExpressionComponentMask.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"

ARaymarchMaterialBuilder::ARaymarchMaterialBuilder()
	:RaymarchedPhysicsShapes{}
	, RaymarchedShapesProperties{}
	, Material{}
{
	PrimaryActorTick.bCanEverTick = true;

	CreateMaterial();
}
void ARaymarchMaterialBuilder::BeginPlay()
{
	Super::BeginPlay();

	if (Material == nullptr) 
	{
		CreateMaterial();
	}
	PopulateMaterial();
}
void ARaymarchMaterialBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMaterial();
}

void ARaymarchMaterialBuilder::CreateMaterial()
{
	FString MaterialBaseName = "M_Material";
	FString PackageName = "/Game/";
	PackageName += MaterialBaseName;
	UPackage* Package = CreatePackage(*PackageName);

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
	Package->SetDirtyFlag(true);
}
void ARaymarchMaterialBuilder::PopulateMaterial()
{
	//Setup lighting
	LightingData.SetupLighting(Material, LightingProperties);

	//Setup universally used variables
	SetupStaticVariables();

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
			RaymarchedPhysicsShapes.Add(properties.CreateShape(Material, *shape, i));
		}
		else 
		{
			check(shape);
		}
	}

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
	for (int i = 0; i < RaymarchedPhysicsShapes.Num(); i++)
	{

	}
}

void ARaymarchMaterialBuilder::SetupStaticVariables()
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

	FRaymarchedShapeProperties temp;
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
	Material->Expressions.Add(mask);
	lerp->A.Expression = mask;
}
