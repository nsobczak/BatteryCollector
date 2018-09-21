// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**Returns where to spawn subobject*/
	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	/**Find a random point within the Boxcomponent*/
	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector	GetRandomPointInVolume();

	/**toggles whether or not the spawn volume spawns pickups*/
	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void SetSpawningActive(bool shouldSpawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**The pickup to spawn*/
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer; //timer can't be properties

	/**minimum spwan delay*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnDelayRangeLow;

	/**maximum spwan delay*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		float SpawnDelayRangeHigh;

private:
	/** Box compnent to specify where pickups should be spawned*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent * WhereToSpawn;

	/**handle spawning a new pickup*/
	void SpawnPickup();

	/**current spawn delay*/
	float CurrentSpawnDelay;

};
