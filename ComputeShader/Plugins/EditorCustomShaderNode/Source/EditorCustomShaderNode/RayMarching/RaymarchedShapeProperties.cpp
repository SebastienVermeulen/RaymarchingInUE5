#include "RaymarchedShapeProperties.h"
#include "../CustomExpression/CustomFileMaterialExpression.h"
#include "Materials/MaterialExpressionScalarParameter.h"
#include "Materials/MaterialExpressionVectorParameter.h"
#include "RaymarchedPhysicsShape.h"
#include "Materials/MaterialExpressionNormalize.h"
#include "Materials/MaterialExpressionCameraPositionWS.h"

UMaterialExpressionNormalize* FRaymarchedShapeProperties::RayDir = nullptr;
UMaterialExpressionCameraPositionWS* FRaymarchedShapeProperties::RayOrig = nullptr;

ARaymarchedPhysicsShape* FRaymarchedShapeProperties::CreateShape(UMaterial* Material, const FShapeShaderProperties shape, const int idx)
{
	//Create the physics object
	ARaymarchedPhysicsShape* physicsShape = ARaymarchedPhysicsShape::Create(Radius);

	physicsShape->SetActorRotation(StartRotation, ETeleportType::TeleportPhysics);
	physicsShape->SetActorLocation(StartPosition, false, nullptr, ETeleportType::TeleportPhysics);

	CreateParameters(Material, idx);

	HookupMarching(Material, shape, idx);
	HookupShading(Material, shape, idx);
	HookupLighting(Material, shape, idx);

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
	ObjectRotationParam->DefaultValue = StartRotation.Vector();
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
	ExpressionMarch->AdditionalOutputs.Empty();
	CustomOutput.OutputName = TEXT("dist");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	ExpressionMarch->AdditionalOutputs.Add(CustomOutput);
	CustomOutput.OutputName = TEXT("lastDist");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	ExpressionMarch->AdditionalOutputs.Add(CustomOutput);
	CustomOutput.OutputName = TEXT("steps");
	CustomOutput.OutputType = ECustomMaterialOutputType::CMOT_Float1;
	ExpressionMarch->AdditionalOutputs.Add(CustomOutput);
#pragma endregion CustomShaders
}
void FRaymarchedShapeProperties::HookupShading(UMaterial* Material, const FShapeShaderProperties shape, const int idx)
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
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrigin");
	CustomInput.Input.Expression = ObjectOriginParam;
	ExpressionShading->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRad");
	CustomInput.Input.Expression = ObjectRadiusParam;
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
	auto inputLambda = [compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; };
	auto outputLambda = [compareOutputStr](FCustomOutput x) { return x.OutputName == compareOutputStr; };

	compareInputStr = "lastPos";
	ExpressionShading->Inputs.FindByPredicate([compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; })->Input.Expression = ExpressionMarch;
	compareInputStr = "dist";
	compareOutputStr = "distance";
	ExpressionShading->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda), ExpressionMarch);
	compareInputStr = "lastDist";
	compareOutputStr = "lastDist";
	ExpressionShading->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda), ExpressionMarch);
	compareInputStr = "steps";
	compareOutputStr = "steps";
	ExpressionShading->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda), ExpressionMarch);
}
void FRaymarchedShapeProperties::HookupLighting(UMaterial* Material, const FShapeShaderProperties shape, const int idx)
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
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeOrigin");
	CustomInput.Input.Expression = ObjectOriginParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("cubeRad");
	CustomInput.Input.Expression = ObjectRadiusParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("shinyness");
	CustomInput.Input.Expression = ShinynessParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("fogMultiplier");
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("diffuseColor");
	CustomInput.Input.Expression = DiffuseColorParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("specularColor");
	CustomInput.Input.Expression = SpecularColorParam;
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("lightColor");
	ExpressionLighting->Inputs.Add(CustomInput);
	CustomInput.InputName = TEXT("ambientColor");
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
#pragma endregion CustomShaders

	FName compareInputStr = "";
	FName compareOutputStr = "";
	auto inputLambda = [compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; };
	auto outputLambda = [compareOutputStr](FCustomOutput x) { return x.OutputName == compareOutputStr; };

	compareInputStr = "lastPos";
	ExpressionLighting->Inputs.FindByPredicate([compareInputStr](FCustomInput x) { return x.InputName == compareInputStr; })->Input.Expression = ExpressionMarch;
	compareInputStr = "dist";
	compareOutputStr = "distance";
	ExpressionLighting->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda), ExpressionMarch);
	compareInputStr = "lastDist";
	compareOutputStr = "lastDist";
	ExpressionLighting->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda), ExpressionMarch);
	compareInputStr = "steps";
	compareOutputStr = "steps";
	ExpressionLighting->Inputs.FindByPredicate(inputLambda)->Input.Connect(ExpressionMarch->AdditionalOutputs.IndexOfByPredicate(outputLambda), ExpressionMarch);
}
