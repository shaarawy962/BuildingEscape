// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Engine/EngineBaseTypes.h"

// Sets default values for this component's properties
UGrabbingComponent::UGrabbingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabbingComponent::BeginPlay()
{
	Super::BeginPlay();
	PhysicsHandleComponent();
	FindInputComponent();
}

void UGrabbingComponent::Release()
{
	if (PhysicsHandle)
	{
		if (PhysicsHandle->GrabbedComponent)
		{
			PhysicsHandle->ReleaseComponent();
		}
	}


	UE_LOG(LogTemp, Display, TEXT("Grab released"))
}

void UGrabbingComponent::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("Grab pressed"))
	auto OutHit = GetFirstBodyInReach();
	ComponentToGrab = OutHit.GetComponent();
	auto HitActor = OutHit.GetActor();
	

	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	GetFirstBodyInReach();
	
    // If we hit something attach physics handle
	// TODO attach a physics handle
	if(HitActor)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(),
			ComponentToGrab->GetOwner()->GetActorRotation());

	}
}

void UGrabbingComponent::PhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		UE_LOG(LogTemp, Display, TEXT("Physics Handle is attached to %s"), *(GetOwner()->GetName()))
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handle"), *(GetOwner()->GetName()))
	}
}

void UGrabbingComponent::FindInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input) {
		UE_LOG(LogTemp, Display, TEXT("Input is attached to %s"), *(GetOwner()->GetName()))
			Input->BindAction("Grab", IE_Pressed, this, &UGrabbingComponent::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabbingComponent::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Input is missing from %s"), *GetOwner()->GetName())
	}
}

const FHitResult UGrabbingComponent::GetFirstBodyInReach()
{
	// Get the player view point
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	LineTraceEndPoint = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	//Setup Query parameters
	FCollisionQueryParams TraceParameter(FName(TEXT("")), false, GetOwner());

	//Line-Trace(AKA Ray-cast) out to reach distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(OUT Hit,
		PlayerViewPointLocation,
		LineTraceEndPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameter);

	AActor* MyActor = Hit.GetActor();

	if (MyActor) {
		FString ActorName = MyActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorName);
	}

	return Hit;
}

// Called every frame
void UGrabbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
		LineTraceEndPoint = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
		PhysicsHandle->SetTargetLocation(LineTraceEndPoint);
	}
	//move grabbed object around

}