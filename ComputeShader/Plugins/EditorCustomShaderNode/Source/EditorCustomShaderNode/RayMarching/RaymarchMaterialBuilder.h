#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaymarchedShapeProperties.h"
#include "ShapeShaderProperties.h"
#include "RaymarchedLightingData.h"
#include "RaymarchMaterialBuilder.generated.h"

class ARaymarchedPhysicsShape;
class UMaterialExpressionLinearInterpolate;
class UPostProcessComponent;
class UMaterialInstanceDynamic;
class UMaterialInstanceConstant;
struct FRaymarchedLightingProperties;

UCLASS()
class ARaymarchMaterialBuilder : public AActor
{
	GENERATED_BODY()

#pragma region Functions
public:
	ARaymarchMaterialBuilder();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void CreateMaterial();
	void PopulateMaterial();
	void UpdateMaterial();

	UMaterialExpressionLinearInterpolate* SetupStaticVariables();
	void SetupConnectingVariables(UMaterialExpressionLinearInterpolate* lerp);

	void CreateMaterialInstance();
	void UpdateDynamicMaterial();

#pragma endregion

#pragma region Variables
public:

protected:

private:
	//Settings and data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Shape Details", Category = "Shapes", meta = (AllowPrivateAccess = true))
	TArray<FRaymarchedShapeProperties> RaymarchedShapesProperties;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Shape Properties", Category = "Shapes", meta = (AllowPrivateAccess = true))
	TArray<FShapeShaderProperties> ShapeShaderProperties;
	//Object during runtime
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, DisplayName = "Physics Shapes", Category = "Shapes", meta = (AllowPrivateAccess = true))
	TArray<ARaymarchedPhysicsShape*> RaymarchedPhysicsShapes;

	FString MaterialBaseName = "M_Material";
	FString MaterialInstanceBaseName = "MI_Material";
	FString PackageName = "/Game/Materials/";
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, DisplayName = "Dynamic Material", Category = "Material", meta = (AllowPrivateAccess = true))
	UMaterial* Material;
	UMaterialInstanceConstant* MaterialInstance;
	UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, DisplayName = "Dynamic Material", Category = "Material", meta = (AllowPrivateAccess = true))
	UPostProcessComponent* PostProcessing;

	//Settings and data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Lighting Properties", Category = "Lighting", meta = (AllowPrivateAccess = true))
	FRaymarchedLightingProperties LightingProperties;
	//Object during runtime
	FRaymarchedLightingData LightingData;
#pragma endregion
};
