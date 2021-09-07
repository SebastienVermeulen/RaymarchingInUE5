#include "RaymarchedPhysicsCube.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

ARaymarchedPhysicsCube::ARaymarchedPhysicsCube(const FObjectInitializer& ObjectInitializer)
	: ARaymarchedPhysicsShape(ObjectInitializer)
	, CollisionMesh{nullptr}
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionMesh->SetupAttachment(GetRootComponent());

	CollisionMesh->bDynamicObstacle = true;
	CollisionMesh->SetSimulatePhysics(true);
	CollisionMesh->bHiddenInGame = true;
	FCollisionResponseContainer cont{};
	cont.SetAllChannels(ECollisionResponse::ECR_Block);
	CollisionMesh->SetCollisionResponseToChannels(cont);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}
void ARaymarchedPhysicsCube::Init(float radius)
{
	CollisionMesh->SetBoxExtent(FVector(radius, radius, radius));
}
void ARaymarchedPhysicsCube::BeginPlay()
{
	Super::BeginPlay();
}
void ARaymarchedPhysicsCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ARaymarchedPhysicsCube::DebugDrawShape()
{
	DrawDebugBox(GetWorld(), GetActorLocation(), CollisionMesh->GetScaledBoxExtent(), GetActorRotation().Quaternion(), FColor::Purple, false, -1, 0, 10);
}
#endif
