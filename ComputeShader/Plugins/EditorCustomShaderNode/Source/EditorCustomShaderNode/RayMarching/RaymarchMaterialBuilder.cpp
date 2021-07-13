//Credit for material creation: https://isaratech.com/ue4-programmatically-create-a-new-material-and-inner-nodes/

#include "RaymarchMaterialBuilder.h"
#include "RaymarchedPhysicsShape.h"
#include "Factories/MaterialFactoryNew.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "../CustomExpression/CustomFileMaterialExpression.h"
#include "Materials/MaterialExpressionScalarParameter.h"

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
			CreateShape(properties, *shape, i);
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

void ARaymarchMaterialBuilder::CreateShape(const FRaymarchedShapeProperties properties, const FShapeShaderProperties shape, const int idx)
{
	//Create the physics object
	ARaymarchedPhysicsShape* physicsShape = ARaymarchedPhysicsShape::Create(properties.Radius);

	physicsShape->SetActorRotation(properties.StartRotation, ETeleportType::TeleportPhysics);
	physicsShape->SetActorLocation(properties.StartPosition, false, nullptr, ETeleportType::TeleportPhysics);
	
	//Add the object to the list
	RaymarchedPhysicsShapes.Add(physicsShape);

#pragma region CustomShaders
	//Creating the shaders
	UCustomFileMaterialExpression* expressionMarch = NewObject<UCustomFileMaterialExpression>(Material);
	expressionMarch->CodeFile = shape.RayMarch;
	Material->Expressions.Add(expressionMarch);
	UCustomFileMaterialExpression* expressionShading = NewObject<UCustomFileMaterialExpression>(Material);
	expressionShading->CodeFile = shape.Shading;
	Material->Expressions.Add(expressionShading);
	UCustomFileMaterialExpression* expressionLighting = NewObject<UCustomFileMaterialExpression>(Material);
	expressionLighting->CodeFile = shape.Lighting;
	Material->Expressions.Add(expressionLighting);

	FCustomInput CustomInput;
	FCustomOutput CustomOutput;
	
	//Custom marching
	expressionMarch->Description = "Raymarch";
	//Inputs
	expressionMarch->Inputs.Empty();
	CustomInput.InputName = TEXT("time");
	expressionMarch->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("rayOrigin");
	expressionMarch->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("rayDirection");
	expressionMarch->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrigin");
	expressionMarch->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRadius");
	expressionMarch->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRotation");
	expressionMarch->Inputs.Add(CustomInput);
	//Outputs
	expressionMarch->OutputType = ECustomMaterialOutputType::CMOT_Float3;
	expressionMarch->AdditionalOutputs.Empty();
	CustomOutput.OutputName = TEXT("dist");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	expressionMarch->AdditionalOutputs.Add(CustomOutput);
	CustomOutput.OutputName = TEXT("lastDist");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	expressionMarch->AdditionalOutputs.Add(CustomOutput);
	CustomOutput.OutputName = TEXT("steps");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	expressionMarch->AdditionalOutputs.Add(CustomOutput);
	//Custom shading
	expressionShading->Description = "Shading";
	//Inputs
	expressionShading->Inputs.Empty();
	CustomInput.InputName = TEXT("lastPos");
	expressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("distance");
	expressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lastDist");
	expressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("steps");
	expressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightOrigin");
	expressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrigin");
	expressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRad");
	expressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("normal");
	expressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightDir");
	expressionShading->Inputs.Add(CustomInput);
	//Outputs
	expressionShading->OutputType = ECustomMaterialOutputType::CMOT_Float1;
	//Custom lighting
	expressionLighting->Description = "lighting";
	//Inputs
	expressionLighting->Inputs.Empty();
	CustomInput.InputName = TEXT("lastPos");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("distance");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lastDist");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("steps");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("rayPos");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("rayDirection");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightOrigin");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrigin");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRad");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("shinyness");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("fogMultiplier");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("diffuseColor");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("specularColor");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightColor");
	expressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("ambientColor");
	expressionLighting->Inputs.Add(CustomInput);
	//Outputs
	expressionLighting->OutputType = ECustomMaterialOutputType::CMOT_Float4;
	expressionLighting->AdditionalOutputs.Empty();
	CustomOutput.OutputName = TEXT("normal");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float3;
	expressionLighting->AdditionalOutputs.Add(CustomOutput);
	CustomOutput.OutputName = TEXT("lightDir");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float3;
	expressionLighting->AdditionalOutputs.Add(CustomOutput);
#pragma endregion CustomShaders

	//Parameters
	UMaterialExpressionScalarParameter* param = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(param);
	param->ParameterName = TEXT("ObjectOrigin_" + idx);
	param = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(param);
	param->ParameterName = TEXT("ObjectRotation_" + idx);
	param = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(param);
	param->ParameterName = TEXT("ObjectRadius_" + idx);
}
