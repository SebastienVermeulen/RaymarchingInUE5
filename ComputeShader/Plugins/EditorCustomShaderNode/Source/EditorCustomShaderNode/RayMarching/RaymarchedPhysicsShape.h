#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaymarchedPhysicsShape.generated.h"

class UBoxComponent;

UCLASS()
class ARaymarchedPhysicsShape : public AActor
{
	GENERATED_BODY()
	
#pragma region Functions
public:	
	ARaymarchedPhysicsShape(const FObjectInitializer& ObjectInitializer);
	void Init(float radius);

	virtual void Tick(float DeltaTime) override;

#if WITH_EDITOR
	void DebugDrawShape();
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
