#include "RaymarchedShapeProperties.h"
#include "../CustomExpression/CustomFileMaterialExpression.h"
#include "RaymarchedPhysicsShape.h"
#include "RaymarchedLightingData.h"
#include "RaymarchMaterialBuilder.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "Materials/MaterialExpressionCameraPositionWS.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "Materials/MaterialExpressionScalarParameter.h"

UMaterialExpressionNormalize* FRaymarchedShapeProperties::RayDir = nullptr;
UMaterialExpressionCameraPositionWS* FRaymarchedShapeProperties::RayOrig = nullptr;

FRaymarchedShapeProperties::FRaymarchedShapeProperties() 
	:ShapeType{}
	, StartPosition{}
	, StartRotation{}
	, Radius{}
	, DiffuseColor{}
	, SpecularColor{}
	, Shinyness{}
#if WITH_EDITOR
	, TotalEditorHeight{1500}
#endif
	, ExpressionMarch{ nullptr }
	, ExpressionShading{ nullptr }
	, ExpressionLighting{ nullptr }
	, ObjectOriginParam{ nullptr }
	, ObjectRotationParam{ nullptr }
	, ObjectRadiusParam{ nullptr }
	, DiffuseColorParam{ nullptr }
	, SpecularColorParam{ nullptr }
	, ShinynessParam{ nullptr }
{	
}

ARaymarchedPhysicsShape* FRaymarchedShapeProperties::CreateShape(ARaymarchMaterialBuilder* Builder, UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx)
{
	//Create the physics object
	FActorSpawnParameters params{};
	params.Name = TEXT("Shape_" + idx);
	params.Owner = Builder;
	AActor* physicsShapeActor = Builder->GetWorld()->SpawnActor(ARaymarchedPhysicsShape::StaticClass(), &StartPosition, &StartRotation, params);
	ARaymarchedPhysicsShape* physicsShape = Cast<ARaymarchedPhysicsShape>(physicsShapeActor);
	physicsShape->Init(Radius);

	CreateParameters(Material, idx);

	HookupMarching(Material, shape, idx);
	HookupShading(Material, shape, lightingData, idx);
	HookupLighting(Material, shape, lightingData, idx);

	return physicsShape;
}

void FRaymarchedShapeProperties::CreateParameters(UMaterial* Material, const int idx)
{
	//Parameters
	ObjectOriginParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(ObjectOriginParam);
	ObjectOriginParam->ParameterName = TEXT("ObjectOrigin_" + idx);
	ObjectOriginParam->DefaultValue = StartPosition;
	ObjectRotationParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(ObjectRotationParam);
	ObjectRotationParam->ParameterName = TEXT("ObjectRotation_" + idx);
	ObjectRotationParam->DefaultValue = FVector(StartRotation.Roll, StartRotation.Pitch, StartRotation.Yaw);
	ObjectRadiusParam = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(ObjectRadiusParam);
	ObjectRadiusParam->ParameterName = TEXT("ObjectRadius_" + idx);
	ObjectRadiusParam->DefaultValue = Radius;

	DiffuseColorParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(DiffuseColorParam);
	DiffuseColorParam->ParameterName = TEXT("DiffuseColor_" + idx);
	DiffuseColorParam->DefaultValue = DiffuseColor;
	SpecularColorParam = NewObject<UMaterialExpressionVectorParameter>(Material);
	Material->Expressions.Add(SpecularColorParam);
	SpecularColorParam->ParameterName = TEXT("SpecularColor_" + idx);
	SpecularColorParam->DefaultValue = SpecularColor;
	ShinynessParam = NewObject<UMaterialExpressionScalarParameter>(Material);
	Material->Expressions.Add(ShinynessParam);
	ShinynessParam->ParameterName = TEXT("Shinyness_" + idx);
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

void FRaymarchedShapeProperties::HookupMarching(UMaterial* Material, const FShapeShaderProperties shape, const int idx)
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
void FRaymarchedShapeProperties::HookupShading(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx)
{
	//Creating the shaders
	ExpressionShading = NewObject<UCustomFileMaterialExpression>(Material);
	ExpressionShading->CodeFile = shape.Shading;
	Material->Expressions.Add(ExpressionShading);

	FCustomInput CustomInput;
	FCustomOutput CustomOutput;

#pragma region CustomShaders
	//Custom shading
	ExpressionShading->Description = "Shading";
	//Inputs
	ExpressionShading->Inputs.Empty();
	CustomInput.InputName = TEXT("lastPos");
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("distance");
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lastDist");
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("steps");
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightOrigin");
	CustomInput.Input.Expression = lightingData.LightOrigin;
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrig");
	CustomInput.Input.Expression = ObjectOriginParam;
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRad");
	CustomInput.Input.Expression = ObjectRadiusParam;
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRotation");
	CustomInput.Input.Expression = ObjectRotationParam;
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("normal");
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightDir");
	ExpressionShading->Inputs.Add(CustomInput);
	//Outputs
	ExpressionShading->OutputType = ECustomMaterialOutputType::CMOT_Float1;
#pragma endregion CustomShaders

	FName compareInputStr = "";
	FName compareOutputStr = "";
	auto inputLambda = [&compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; };
	auto outputLambda = [&compareOutputStr](FCustomOutput x) { return x.OutputName == compareOutputStr; };

	//We add 1 to the index due to us searching in only the additional outputs, so we need to take into account the original 0th element (the original output)
	compareInputStr = "lastPos";
	ExpressionShading->Inputs.FindByPredicate([compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; })->Input.Expression = ExpressionMarch;
	compareInputStr = "distance";
	compareOutputStr = "dist";
	ExpressionShading->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionMarch);
	compareInputStr = "lastDist";
	compareOutputStr = "lastDist";
	ExpressionShading->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionMarch);
	compareInputStr = "steps";
	compareOutputStr = "steps";
	ExpressionShading->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionMarch);

#if WITH_EDITOR
	float editorOffset = TotalEditorHeight * idx;

	ExpressionShading->MaterialExpressionEditorX = 0;
	ExpressionShading->MaterialExpressionEditorY = editorOffset;
#endif
}
void FRaymarchedShapeProperties::HookupLighting(UMaterial* Material, const FShapeShaderProperties shape, const FRaymarchedLightingData lightingData, const int idx)
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
	CustomInput.Input.Expression = lightingData.LightOrigin;
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
	CustomInput.Input.Expression = lightingData.FogMultiplier;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("diffuseColor");
	CustomInput.Input.Expression = DiffuseColorParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("specularColor");
	CustomInput.Input.Expression = SpecularColorParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightColor");
	CustomInput.Input.Expression = lightingData.LightColor;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("ambientColor");
	CustomInput.Input.Expression = lightingData.AmbientColor;
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

	compareInputStr = "normal";
	compareOutputStr = "normal";
	ExpressionShading->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionLighting->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionLighting);	
	compareInputStr = "lightDir";
	compareOutputStr = "lightDir";
	ExpressionShading->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionLighting->AdditionalOutputs.IndexOfByPredicate(outputLambda) + 1, ExpressionLighting);


#if WITH_EDITOR
	float editorOffset = TotalEditorHeight * idx;
	float localTotalOffset = ExpressionShading->GetHeight() + 150.0f;

	ExpressionLighting->MaterialExpressionEditorX = 0;
	ExpressionLighting->MaterialExpressionEditorY = localTotalOffset + editorOffset;
#endif
}

void FRaymarchedShapeProperties::UpdateShape(UMaterialInstanceDynamic* Material, const ARaymarchedPhysicsShape* shape, const int idx)
{
	Material->SetVectorParameterValue("ObjectOrigin_" + idx, shape->GetActorLocation());
	FRotator rot = shape->GetActorRotation();
	FVector radianAngles = FMath::DegreesToRadians(rot.Euler());
	Material->SetVectorParameterValue("ObjectRotation_" + idx, radianAngles);
}
