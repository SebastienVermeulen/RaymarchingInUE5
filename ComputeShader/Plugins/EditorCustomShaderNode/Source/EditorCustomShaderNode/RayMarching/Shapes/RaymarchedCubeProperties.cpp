#include "RaymarchedCubeProperties.h"
#include "../../CustomExpression/CustomFileMaterialExpression.h"
#include "../PhysicsShapes/RaymarchedPhysicsCube.h"
#include "../RaymarchedLightingData.h"
#include "../RaymarchMaterialBuilder.h"
#include "DrawDebugHelpers.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Materials/MaterialExpressionCameraPositionWS.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionLinearInterpolate.h"

URaymarchedCubeProperties::URaymarchedCubeProperties()
	:URaymarchedShapeProperties()
	, IRaymarchedShapeInterface()
	, Radius{}
{	
	ShapeType = TEnumAsByte<Shape>(Shape::Cube);
}

ARaymarchedPhysicsShape* URaymarchedCubeProperties::CreateShape(ARaymarchMaterialBuilder* Builder, UMaterial* Material, const int idx)
{
	//Create the physics object
	FActorSpawnParameters params{};
	FString base = "Shape_";
	FString indexStr = FString::FromInt(idx);
	base.Append(indexStr);
	params.Name = FName(base);
	params.Owner = Builder;
	AActor* physicsShapeActor = Builder->GetWorld()->SpawnActor(ARaymarchedPhysicsCube::StaticClass(), &StartPosition, &StartRotation, params);
	ARaymarchedPhysicsCube* physicsCube = Cast<ARaymarchedPhysicsCube>(physicsShapeActor);
	physicsCube->Init(Radius);
	ARaymarchedPhysicsShape* physicsShape = Cast<ARaymarchedPhysicsShape>(physicsShapeActor);

	return physicsShape;
}

void URaymarchedCubeProperties::CreateParameters(UMaterial* Material, const int idx)
{
	FString base = "";
	FString indexStr = FString::FromInt(idx);

	//Parameters
	ObjectOriginParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(ObjectOriginParam);
	base = "ObjectOrigin_";
	base.Append(indexStr);
	ObjectOriginParam->ParameterName = FName(base);
	ObjectOriginParam->DefaultValue = StartPosition;
	ObjectRotationParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(ObjectRotationParam);
	base = "ObjectRotation_";
	base.Append(indexStr);
	ObjectRotationParam->ParameterName = FName(base);
	ObjectRotationParam->DefaultValue = FVector(StartRotation.Roll, StartRotation.Pitch, StartRotation.Yaw);
	ObjectRadiusParam = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(ObjectRadiusParam);
	base = "ObjectRadius_";
	base.Append(indexStr);
	ObjectRadiusParam->ParameterName = FName(base);
	ObjectRadiusParam->DefaultValue = Radius;

	DiffuseColorParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(DiffuseColorParam);
	base = "DiffuseColor_";
	base.Append(indexStr);
	DiffuseColorParam->ParameterName = FName(base);
	DiffuseColorParam->DefaultValue = DiffuseColor;
	SpecularColorParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(SpecularColorParam);
	base = "SpecularColor_";
	base.Append(indexStr);
	SpecularColorParam->ParameterName = FName(base);
	SpecularColorParam->DefaultValue = SpecularColor;
	ShinynessParam = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(ShinynessParam);
	base = "Shinyness_";
	base.Append(indexStr);
	ShinynessParam->ParameterName = FName(base);
	ShinynessParam->DefaultValue = Shinyness;

#if WITH_EDITOR
	float editorOffset = TotalEditorHeight * idx;
	float localTotalOffset = 0.0f;

	ObjectOriginParam->MaterialExpressionEditorX = -1000;
	ObjectOriginParam->MaterialExpressionEditorY = localTotalOffset + editorOffset;
	localTotalOffset += ObjectOriginParam->GetHeight() + 50.0f;
	ObjectRotationParam->MaterialExpressionEditorX = -1000;
	ObjectRotationParam->MaterialExpressionEditorY = localTotalOffset + editorOffset;
	localTotalOffset += ObjectRotationParam->GetHeight() + 50.0f;
	ObjectRadiusParam->MaterialExpressionEditorX = -1000;
	ObjectRadiusParam->MaterialExpressionEditorY = localTotalOffset + editorOffset;
	localTotalOffset += ObjectRadiusParam->GetHeight() + 100.0f;

	DiffuseColorParam->MaterialExpressionEditorX = -1000;
	DiffuseColorParam->MaterialExpressionEditorY = localTotalOffset + editorOffset;
	localTotalOffset += DiffuseColorParam->GetHeight() + 50.0f;
	SpecularColorParam->MaterialExpressionEditorX = -1000;
	SpecularColorParam->MaterialExpressionEditorY = localTotalOffset + editorOffset;
	localTotalOffset += SpecularColorParam->GetHeight() + 50.0f;
	ShinynessParam->MaterialExpressionEditorX = -1000;
	ShinynessParam->MaterialExpressionEditorY = localTotalOffset + editorOffset;
#endif
}

void URaymarchedCubeProperties::HookupMarching(UMaterial* Material, const FShapeShaderProperties shape, const int idx)
{
	//Creating the shaders
	ExpressionMarch = NewObject<UCustomFileMaterialExpression>(Material);
	ExpressionMarch->CodeFile = shape.RayMarch;
	Material->Expressions.Add(ExpressionMarch);

	FCustomInput CustomInput;
	FCustomOutput CustomOutput;

#pragma region CustomShaders
	//Custom marching
	ExpressionMarch->Description = "Raymarch";
	//Inputs
	ExpressionMarch->Inputs.Empty();
	CustomInput.Input.Expression = RayOrig;
	CustomInput.InputName = TEXT("rayOrigin");
	ExpressionMarch->Inputs.Add(CustomInput);
	CustomInput.Input.Expression = RayDir;
	CustomInput.InputName = TEXT("rayDirection");
	ExpressionMarch->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrigin");
	CustomInput.Input.Expression = ObjectOriginParam;
	ExpressionMarch->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRadius");
	CustomInput.Input.Expression = ObjectRadiusParam;
	ExpressionMarch->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRotation");
	CustomInput.Input.Expression = ObjectRotationParam;
	ExpressionMarch->Inputs.Add(CustomInput);
	//Outputs
	ExpressionMarch->OutputType = ECustomMaterialOutputType::CMOT_Float3;
	CustomOutput.OutputName = TEXT("dist");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	ExpressionMarch->AdditionalOutputs.Add(CustomOutput);
	CustomOutput.OutputName = TEXT("lastDist");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	ExpressionMarch->AdditionalOutputs.Add(CustomOutput);
	CustomOutput.OutputName = TEXT("steps");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	ExpressionMarch->AdditionalOutputs.Add(CustomOutput);
	ExpressionMarch->RebuildOutputs();
#pragma endregion CustomShaders

#if WITH_EDITOR
	float editorOffset = TotalEditorHeight * idx;
	float localTotalOffset = ExpressionMarch->GetHeight() + 50.0f;

	ExpressionMarch->MaterialExpressionEditorX = -700;
	ExpressionMarch->MaterialExpressionEditorY = editorOffset;
#endif
}
void URaymarchedCubeProperties::HookupShading(UMaterial* Material, const TArray<URaymarchedShapeProperties*> raymarchedShapesProperties,
	const TArray<FShapeShaderProperties> shapes, const FRaymarchedLightingData lightingData, const int idx, const int nrShapes)
{
	TEnumAsByte<Shape> type;
	const FShapeShaderProperties* shapeProp;

	//Creating the shaders nodes
	for (int i = 0; i < nrShapes; i++)
	{
		//Find the correct shaderproperties
		type = raymarchedShapesProperties[i]->ShapeType;
		shapeProp = shapes.FindByPredicate([type](FShapeShaderProperties shapeProp) 
			{ 
				return shapeProp.ShapeType == type; 
			});

		//Use the correct shape to create the specific shader node with specific inputs
		ExpressionShading.Add(Cast<IRaymarchedShapeInterface>(raymarchedShapesProperties[i])->CreateShadingNode(
				Material, *shapeProp, lightingData, idx, i));
		//We add 1 to the index due to us searching in only the additional outputs, so we need to take into account the original 0th element (the original output)
		FName compareInputStr = "";
		compareInputStr = "lastPos";
		ExpressionShading.Last()->Inputs.FindByPredicate([compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; })->Input.Expression = ExpressionMarch;
	}

	if (ExpressionLighting != nullptr)
	{
		//Hookup lighting to shading
		FName compareInputStr = "";
		FName compareOutputStr = "";
		auto inputLambda = [&compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; };
		auto outputLambda = [&compareOutputStr](FCustomOutput x) { return x.OutputName == compareOutputStr; };
		for (int i = 0; i < nrShapes; i++)
		{
			compareInputStr = "normal";
			compareOutputStr = "normal";
			ExpressionShading[i]->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionLighting->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionLighting);
			compareInputStr = "lightDir";
			compareOutputStr = "lightDir";
			ExpressionShading[i]->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionLighting->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionLighting);
		}
	}
}
void URaymarchedCubeProperties::HookupLighting(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx, const int nrShapes)
{
	ExpressionLighting = NewObject<UCustomFileMaterialExpression>(Material);
	ExpressionLighting->CodeFile = shape.Lighting;
	Material->Expressions.Add(ExpressionLighting);

	FCustomInput CustomInput;
	FCustomOutput CustomOutput;

#pragma region CustomShaders
	//Custom lighting
	ExpressionLighting->Description = "lighting";
	//Inputs
	ExpressionLighting->Inputs.Empty();
	CustomInput.InputName = TEXT("lastPos");
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("distance");
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lastDist");
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("steps");
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.Input.Expression = RayOrig;
	CustomInput.InputName = TEXT("rayPos");
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.Input.Expression = RayDir;
	CustomInput.InputName = TEXT("rayDirection");
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightOrigin");
	CustomInput.Input.Expression = lightingData.LightOriginParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightStrength");
	CustomInput.Input.Expression = lightingData.LightStrengthParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrig");
	CustomInput.Input.Expression = ObjectOriginParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRad");
	CustomInput.Input.Expression = ObjectRadiusParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRotation");
	CustomInput.Input.Expression = ObjectRotationParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("shinyness");
	CustomInput.Input.Expression = ShinynessParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("fogMultiplier");
	CustomInput.Input.Expression = lightingData.FogMultiplierParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("diffuseColor");
	CustomInput.Input.Expression = DiffuseColorParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("specularColor");
	CustomInput.Input.Expression = SpecularColorParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightColor");
	CustomInput.Input.Expression = lightingData.LightColorParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("ambientColor");
	CustomInput.Input.Expression = lightingData.AmbientColorParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	//Outputs
	ExpressionLighting->OutputType = ECustomMaterialOutputType::CMOT_Float4;
	ExpressionLighting->AdditionalOutputs.Empty();
	CustomOutput.OutputName = TEXT("normal");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float3;
	ExpressionLighting->AdditionalOutputs.Add(CustomOutput);
	CustomOutput.OutputName = TEXT("lightDir");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float3;
	ExpressionLighting->AdditionalOutputs.Add(CustomOutput);
	ExpressionLighting->RebuildOutputs();
#pragma endregion CustomShaders

	FName compareInputStr = "";
	FName compareOutputStr = "";
	auto inputLambda = [&compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; };
	auto outputLambda = [&compareOutputStr](FCustomOutput x) { return x.OutputName == compareOutputStr; };

	//We add 1 to the index due to us searching in only the additional outputs, so we need to take into account the original 0th element (the original output)
	compareInputStr = "lastPos";
	ExpressionLighting->Inputs.FindByPredicate([compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; })->Input.Expression = ExpressionMarch;
	compareInputStr = "distance";
	compareOutputStr = "dist";
	ExpressionLighting->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionMarch);
	compareInputStr = "lastDist";
	compareOutputStr = "lastDist";
	ExpressionLighting->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionMarch);
	compareInputStr = "steps";
	compareOutputStr = "steps";
	ExpressionLighting->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionMarch);

#if WITH_EDITOR
	float editorOffset = TotalEditorHeight * idx;

	ExpressionLighting->MaterialExpressionEditorX = 0;
	ExpressionLighting->MaterialExpressionEditorY = editorOffset + TotalEditorHeight - (ExpressionLighting->GetHeight() + 350.0f);
#endif
}
void URaymarchedCubeProperties::HookUpOtherShading(UCustomFileMaterialExpression* Shading)
{
	//Inputs
	FName compareInputStr = "";
	compareInputStr = "cubeOrig";
	Shading->Inputs.FindByPredicate([compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; })->Input.Expression = ObjectOriginParam;
	compareInputStr = "cubeRad";
	Shading->Inputs.FindByPredicate([compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; })->Input.Expression = ObjectRadiusParam;
	compareInputStr = "cubeRotation";
	Shading->Inputs.FindByPredicate([compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; })->Input.Expression = ObjectRotationParam;
}

void URaymarchedCubeProperties::UpdateShape(UMaterialInstanceDynamic* Material, const ARaymarchedPhysicsShape* shape, const int idx)
{
	FString base = "";
	FString indexStr = FString::FromInt(idx);

	base = "ObjectOrigin_";
	base.Append(indexStr);
	Material->SetVectorParameterValue(FName(base), shape->GetActorLocation());

	FRotator rot = shape->GetActorRotation();
	FVector radianAngles = FMath::DegreesToRadians(rot.Euler());
	base = "ObjectRotation_";
	base.Append(indexStr);
	Material->SetVectorParameterValue(FName(base), radianAngles);
}

#if WITH_EDITOR
/// <summary>
/// To be called before generating. Will not adjust anything afterwards.
/// </summary>
/// <param name="nrOfShapes">The number of shapes being created in the world.</param>
void URaymarchedCubeProperties::AdjustEditorHeight(const int nrOfShapes, const float baseheight, const float shadowShaderHeight)
{
	TotalEditorHeight = baseheight + shadowShaderHeight * nrOfShapes;
}

void URaymarchedCubeProperties::DebugDrawShape(UWorld* World)
{
	DrawDebugBox(World, StartPosition, FVector(Radius), StartRotation.Quaternion(), DiffuseColor, false, -1, 0, 10);
}
#endif

UCustomFileMaterialExpression* URaymarchedCubeProperties::CreateShadingNode(UMaterial* Material, const FShapeShaderProperties shape,
	const FRaymarchedLightingData lightingData, const int idx, const int shadeNodeIdx) const
{
	UCustomFileMaterialExpression* expression = NewObject<UCustomFileMaterialExpression>(Material);
	expression->CodeFile = shape.Shading;
	Material->Expressions.Add(expression);
	//Custom shading
	expression->Description = "Shading";

	FCustomInput CustomInput;
	FCustomOutput CustomOutput;

#pragma region CustomShaders
	//Inputs
	expression->Inputs.Empty();
	CustomInput.InputName = TEXT("lastPos");
	expression->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightOrigin");
	CustomInput.Input.Expression = lightingData.LightOriginParam;
	expression->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrig");
	CustomInput.Input.Expression = ObjectOriginParam;
	expression->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRad");
	CustomInput.Input.Expression = ObjectRadiusParam;
	expression->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRotation");
	CustomInput.Input.Expression = ObjectRotationParam;
	expression->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("normal");
	expression->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightDir");
	expression->Inputs.Add(CustomInput);
	//Outputs
	expression->OutputType = ECustomMaterialOutputType::CMOT_Float1;
#pragma endregion CustomShaders

#if WITH_EDITOR
	float editorOffset = TotalEditorHeight * idx;

	expression->MaterialExpressionEditorX = 0;
	expression->MaterialExpressionEditorY = editorOffset + (expression->GetHeight() + 150.0f) * shadeNodeIdx;
#endif

	return expression;
}
