#include "RaymarchedPhysicsSphere.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"

ARaymarchedPhysicsSphere::ARaymarchedPhysicsSphere(const FObjectInitializer& ObjectInitializer)
	: ARaymarchedPhysicsShape(ObjectInitializer)
	, CollisionMesh{nullptr}
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionMesh->SetupAttachment(GetRootComponent());

	CollisionMesh->bDynamicObstacle = true;
	CollisionMesh->SetSimulatePhysics(true);
	CollisionMesh->bHiddenInGame = true;
	FCollisionResponseContainer cont{};
	cont.SetAllChannels(ECollisionResponse::ECR_Block);
	CollisionMesh->SetCollisionResponseToChannels(cont);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}
void ARaymarchedPhysicsSphere::Init(float radius)
{
	CollisionMesh->SetSphereRadius(radius);
}
void ARaymarchedPhysicsSphere::BeginPlay()
{
	Super::BeginPlay();

}
void ARaymarchedPhysicsSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ARaymarchedPhysicsSphere::DebugDrawShape()
{
	DrawDebugCircle(GetWorld(), GetActorLocation(), CollisionMesh->GetScaledSphereRadius(), 30, FColor::Purple, false, -1, 0, 10, FVector(1, 0, 0), FVector(0, 1, 0), false);
	DrawDebugCircle(GetWorld(), GetActorLocation(), CollisionMesh->GetScaledSphereRadius(), 30, FColor::Purple, false, -1, 0, 10, FVector(1, 0, 0), FVector(0, 0, 1), false);
	DrawDebugCircle(GetWorld(), GetActorLocation(), CollisionMesh->GetScaledSphereRadius(), 30, FColor::Purple, false, -1, 0, 10, FVector(0, 1, 0), FVector(0, 0, 1), false);
}
#endif
