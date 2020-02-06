// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabbingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
	FColor Colour = FColor(250, 0, 0);
	// ...
	
}


// Called every frame
void UGrabbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,OUT PlayerViewPointRotation);

	//*LineTraceDirection = PlayerViewPointRotation.Vector();
	LineTraceEndPoint = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(GetWorld()
		, PlayerViewPointLocation
		, LineTraceEndPoint
		, FColor(255, 0, 0)
		, false
		, -1.0f
		, (uint8)'\000'
		, 15.f);

	UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),

	*PlayerViewPointLocation.ToString(),
	*PlayerViewPointRotation.ToString()
	)

		///Setup Query parameters
		FCollisionQueryParams TraceParameter(FName(TEXT("")), false,GetOwner());
	//Line-Trace(AKA Ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit,
	PlayerViewPointLocation,
	LineTraceEndPoint, 
	FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
	TraceParameter);
	AActor * MyActor = Hit.GetActor();
	if(MyActor){
		FString ActorName = MyActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorName);
	}
}

