#include "RaymarchedPhysicsShape.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

ARaymarchedPhysicsShape::ARaymarchedPhysicsShape(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
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
void ARaymarchedPhysicsShape::Init(float radius)
{
	CollisionMesh->SetBoxExtent(FVector(radius, radius, radius));
}
void ARaymarchedPhysicsShape::BeginPlay()
{
	Super::BeginPlay();

}
void ARaymarchedPhysicsShape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void ARaymarchedPhysicsShape::DebugDrawShape()
{
	DrawDebugBox(GetWorld(), GetActorLocation(), CollisionMesh->GetScaledBoxExtent(), GetActorRotation().Quaternion(), FColor::Purple, false, -1, 0, 10);
}
#endif
