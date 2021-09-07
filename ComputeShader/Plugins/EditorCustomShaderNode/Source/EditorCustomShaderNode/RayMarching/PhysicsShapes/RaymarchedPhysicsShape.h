#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaymarchedPhysicsShape.generated.h"

class UBoxComponent;

UCLASS(Abstract)
class ARaymarchedPhysicsShape : public AActor
{
	GENERATED_BODY()
	
#pragma region Functions
public:	
	ARaymarchedPhysicsShape(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR
	virtual void DebugDrawShape() PURE_VIRTUAL(ARaymarchedPhysicsShape::DebugDrawShape, ;);
#endif

protected:

private:

#pragma endregion

#pragma region Variables
public:

protected:

private:

#pragma endregion
};
