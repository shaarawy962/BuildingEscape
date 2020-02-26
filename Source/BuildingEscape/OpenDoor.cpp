// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts	
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
}

void UOpenDoor::ClosesDoor() {
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

void UOpenDoor::OpensDoor()
{
	//Owner->SetActorRotation(FRotator(0.0f, -OpenAngle, 0.0f));
	OnOpenRequest.Broadcast();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(!PressurePlate){
		UE_LOG(LogTemp, Error, TEXT("Door is not triggered by a pressure plate"))
		return;
	}
	else{
		if(GetTotalMassOfActorsOnPlate() > 40.f){
			//OpenDoor();
			OpensDoor();
			LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		}
		if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > Delay) {
			ClosesDoor();
		}
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* Actor : OverlappingActors)
	{
		// Calculate total mass to return
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on pressure plate"), *Actor->GetName())
	}
	return TotalMass;
}

