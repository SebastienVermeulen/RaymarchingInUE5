#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shapes/RaymarchedShapeProperties.h"
#include "RaymarchedLightingProperties.h"
#include "ShapeShaderProperties.h"
#include "RaymarchedLightingData.h"
#include "RaymarchMaterialBuilder.generated.h"

class ARaymarchedPhysicsShape;
class UMaterialExpressionLinearInterpolate;
class UPostProcessComponent;
class UMaterialInstanceDynamic;
class UMaterialInstanceConstant;

UCLASS()
class ARaymarchMaterialBuilder : public AActor
{
	GENERATED_BODY()

#pragma region Functions
public:
	ARaymarchMaterialBuilder();
	virtual void Tick(float DeltaTime) override;
	void EditorTick(float DeltaTime);

	//https://chriszuko.com/ue4-how-to-editor-tick/
	virtual bool ShouldTickIfViewportsOnly() const override;

protected:
	virtual void BeginPlay() override;

private:
	void CreateMaterial();
	void PopulateMaterial();
	void UpdateMaterial();
#if WITH_EDITOR
	void DebugDrawShapes();
#endif

	UMaterialExpressionLinearInterpolate* SetupStaticVariables();
	void SetupConnectingVariables(UMaterialExpressionLinearInterpolate* lerp);

	void CreateMaterialInstance();
	void UpdateDynamicMaterial();

#pragma endregion

#pragma region Variables
public:
#if WITH_EDITOR
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Show Preview", Category = "Shapes")
	bool bShowPreview;
#endif

protected:

private:
	//Settings and data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Shape Details", Category = "Shapes", meta = (AllowPrivateAccess = true))
	TArray<URaymarchedShapeProperties*> RaymarchedShapesProperties;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Shape Properties", Category = "Shapes", meta = (AllowPrivateAccess = true))
	TArray<FShapeShaderProperties> ShapeShaderProperties;
	//Object during runtime
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, DisplayName = "Physics Shapes", Category = "Shapes", meta = (AllowPrivateAccess = true))
	TArray<ARaymarchedPhysicsShape*> RaymarchedPhysicsShapes;

	FString MaterialBaseName;
	FString MaterialInstanceBaseName;
	FString PackageName;
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
