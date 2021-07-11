// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaymarchedPhysicsShape.generated.h"

UCLASS()
class COMPUTESHADER_API ARaymarchedPhysicsShape : public AActor
{
	GENERATED_BODY()
	
#pragma region Functions
public:	
	// Sets default values for this actor's properties
	ARaymarchedPhysicsShape();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

#pragma endregion

#pragma region Variables
public:

protected:

private:
	UBoxComponent* CollisionMesh;

	FVector StartPosition);
	FRotator StartRotation;
	float Radius;
#pragma endregion
};
