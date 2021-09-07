#pragma once

#include "CoreMinimal.h"
#include "RaymarchedPhysicsShape.h"
#include "RaymarchedPhysicsSphere.generated.h"

class USphereComponent;

UCLASS()
class ARaymarchedPhysicsSphere : public ARaymarchedPhysicsShape
{
	GENERATED_BODY()
	
#pragma region Functions
public:	
	ARaymarchedPhysicsSphere(const FObjectInitializer& ObjectInitializer);
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
	USphereComponent* CollisionMesh;
#pragma endregion
};
