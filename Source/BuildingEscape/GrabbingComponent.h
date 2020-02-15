// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GrabbingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabbingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabbingComponent();
	
	UPROPERTY(EditAnywhere)
	FRotator PlayerViewPointRotation;
	FVector PlayerViewPointLocation;
	FVector* LineTraceDirection;
	FVector LineTraceEndPoint;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

private:
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* Input = nullptr;
	UPrimitiveComponent* ComponentToGrab = nullptr;

	void Release();
	void Grab();
	// Look for (assumed) attached Physics Handle Component
	void PhysicsHandleComponent();
	// Look for (assumed) attached input component
	void FindInputComponent();
	//Return Hit for the physics body in reach
	const FHitResult GetFirstBodyInReach();
};
