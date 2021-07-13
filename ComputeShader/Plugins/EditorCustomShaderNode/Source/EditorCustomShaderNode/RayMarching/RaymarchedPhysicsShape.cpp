#include "RaymarchedPhysicsShape.h"
#include "Components/BoxComponent.h"

ARaymarchedPhysicsShape* ARaymarchedPhysicsShape::Create(float radius)
{
	ARaymarchedPhysicsShape* shape = NewObject<ARaymarchedPhysicsShape>();
	shape->Init(radius);
	return shape;
}

ARaymarchedPhysicsShape::ARaymarchedPhysicsShape()
	: CollisionMesh{nullptr}
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionMesh->bDynamicObstacle = true;
	CollisionMesh->SetupAttachment(GetRootComponent());
}
void ARaymarchedPhysicsShape::Init(float radius)
{
	CollisionMesh->SetBoxExtent(FVector(radius, radius, radius));
}

void ARaymarchedPhysicsShape::BeginPlay()
{
	Super::BeginPlay();
}
