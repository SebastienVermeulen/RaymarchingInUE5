#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaymarchedShapeProperties.h"
#include "ShapeShaderProperties.h"
#include "RaymarchedLightingData.h"
#include "RaymarchMaterialBuilder.generated.h"

class ARaymarchedPhysicsShape;
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

	void SetupStaticVariables();
#pragma endregion

#pragma region Variables
public:

protected:

private:
	//Settings and data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FRaymarchedShapeProperties> RaymarchedShapesProperties;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FShapeShaderProperties> ShapeShaderProperties;
	//Object during runtime
	TArray<ARaymarchedPhysicsShape*> RaymarchedPhysicsShapes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UMaterial* Material;

	//Settings and data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FRaymarchedLightingProperties LightingProperties;
	//Object during runtime
	FRaymarchedLightingData LightingData;
#pragma endregion
};
