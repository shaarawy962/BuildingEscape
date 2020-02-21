// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	FRotator* NewRotation;

protected:
	//void OpenDoor();
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void OpensDoor();
	void ClosesDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = 65.f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume *PressurePlate;
	AActor* Owner;
	UPROPERTY(EditAnywhere)
	float Delay = 1.f;
	float LastDoorOpenTime;

	//Gets total mass on plate in kg
	float GetTotalMassOfActors();
};
