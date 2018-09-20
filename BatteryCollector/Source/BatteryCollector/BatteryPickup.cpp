// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"
#include "Classes/Components/StaticMeshComponent.h" 

// Sets default values for this actor's properties
ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);

	//base power level of the battery
	BatteryPower = 150.0f;

}

void ABatteryPickup::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();

	Destroy();
}

//report the power level of the battery
float ABatteryPickup::GetPower()
{
	return BatteryPower;
}
