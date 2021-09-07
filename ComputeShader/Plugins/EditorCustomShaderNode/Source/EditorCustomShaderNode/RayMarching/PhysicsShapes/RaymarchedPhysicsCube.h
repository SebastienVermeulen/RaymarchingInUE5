#pragma once

#include "CoreMinimal.h"
#include "RaymarchedPhysicsShape.h"
#include "RaymarchedPhysicsCube.generated.h"

class UBoxComponent;

UCLASS()
class ARaymarchedPhysicsCube : public ARaymarchedPhysicsShape
{
	GENERATED_BODY()
	
#pragma region Functions
public:	
	ARaymarchedPhysicsCube(const FObjectInitializer& ObjectInitializer);
	void Init(float radius);

	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	virtual void DebugDrawShape() override;
#endif

protected:
	virtual void BeginPlay() override;

private:

#pragma endregion

#pragma region Variables
public:

protected:

private:
	UBoxComponent* CollisionMesh;
#pragma endregion
};
