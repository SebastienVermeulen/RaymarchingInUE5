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
	static ARaymarchedPhysicsShape* Create(float radius);

protected:
	ARaymarchedPhysicsShape();
	void Init(float radius);

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
