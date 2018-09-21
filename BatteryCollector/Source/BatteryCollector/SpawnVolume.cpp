// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include "Classes/Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Pickup.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create BoxComponent
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = (USceneComponent*)WhereToSpawn;

	//set spawn delay range
	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 4.5f;

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector	ASpawnVolume::GetRandomPointInVolume()
{
	FVector spawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector spawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(spawnOrigin, spawnExtent);
}

void ASpawnVolume::SetSpawningActive(bool shouldSpawn)
{
	if (shouldSpawn)
	{
		//set the timer on spawn pickup
		CurrentSpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, CurrentSpawnDelay, false);
	}
	else
	{
		//clear the timer on spawn pickup
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}

void ASpawnVolume::SpawnPickup()
{
	//If we have set smthg to spawn
	if (NULL != WhatToSpawn)
	{
		//check for a valid world => editor can crash if we try to access world and it is null
		UWorld* const world = GetWorld();
		if (world)
		{
			//set the spawn parameters
			FActorSpawnParameters spawnParam;
			spawnParam.Owner = this;
			spawnParam.Instigator = Instigator; //who cause whatToSpawn to be spawned

			//get random location to spawn at
			FVector spawnLocation = GetRandomPointInVolume();
			//get random rotation to spawn at
			FRotator spawnRotation;
			spawnRotation.Yaw = FMath::FRand()*360.0f;
			spawnRotation.Pitch = FMath::FRand()*360.0f;
			spawnRotation.Roll = FMath::FRand()*360.0f;

			//spawn the pickup
			APickup* const spawnPickup = world->SpawnActor<APickup>(WhatToSpawn, spawnLocation, spawnRotation, spawnParam);

			//reset timer
			CurrentSpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, CurrentSpawnDelay, false);
		}
	}

}
