#include "RaymarchedPhysicsShape.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

ARaymarchedPhysicsShape::ARaymarchedPhysicsShape(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}
