#include "RaymarchedPhysicsShape.h"
#include "Components/BoxComponent.h"

ARaymarchedPhysicsShape::ARaymarchedPhysicsShape()
	: CollisionMesh{nullptr}
	, StartPosition{FVector::ZeroVector}
	, StartRotation{FRotator::ZeroRotator}
	, Radius{100.f}
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("TestCollision"));
	CollisionMesh->SetBoxExtent(FVector(Radius, Radius, Radius));
	CollisionMesh->bDynamicObstacle = true;
	CollisionMesh->SetupAttachment(GetRootComponent());

	
}

void ARaymarchedPhysicsShape::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARaymarchedPhysicsShape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
