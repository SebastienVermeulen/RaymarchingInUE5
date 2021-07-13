#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaymarchedShapeProperties.h"
#include "ShapeShaderProperties.h"
#include "RaymarchMaterialBuilder.generated.h"

class ARaymarchedPhysicsShape;
class UMaterial;

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

	void CreateShape(const FRaymarchedShapeProperties properties, const FShapeShaderProperties shape, const int idx);
#pragma endregion

#pragma region Variables
public:

protected:

private:
	TArray<ARaymarchedPhysicsShape*> RaymarchedPhysicsShapes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FRaymarchedShapeProperties> RaymarchedShapesProperties;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FShapeShaderProperties> ShapeShaderProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UMaterial* Material;
#pragma endregion
};
