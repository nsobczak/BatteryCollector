// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h" //to be able to access the character

#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//base decay rate
	DecayRate = 0.01f;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	//find all spawnVolumes
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), foundActors);
	for (size_t i = 0; i < foundActors.Num(); ++i)
	{
		AActor* currentActor = foundActors[i];
		ASpawnVolume* currentSpawnVolumeActor = Cast<ASpawnVolume>(currentActor);
		if (currentSpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(currentSpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

	//set the score to beat
	ABatteryCollectorCharacter* mCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (mCharacter)
	{
		PowerToWin = (mCharacter->GetInitialPower())*1.25f;
	}

	//hud
	if (nullptr != HUDClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (nullptr != CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void ABatteryCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABatteryCollectorCharacter* mCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (mCharacter)
	{
		if (mCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(EBatteryPlayState::EWon);
		}
		else if (mCharacter->GetCurrentPower() > 0)
		{
			mCharacter->UpdatePower(-DeltaTime * DecayRate *(mCharacter->GetInitialPower()));
		}
		else //if negative
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
		}
	}
}
//SetCurrentState(EBatteryPlayState::EPlaying);


float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState()const
{
	return Currentstate;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState newState)
{
	Currentstate = newState;
	HandleNewState(Currentstate);
}

void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState newState)
{
	switch (newState)
	{
	case EBatteryPlayState::EPlaying: {
		//spawn volumes active
		for (size_t i = 0; i < SpawnVolumeActors.Num(); ++i)
		{
			ASpawnVolume* volume = SpawnVolumeActors[i];
			volume->SetSpawningActive(true);
		}
		break;
	}

	case EBatteryPlayState::EGameOver:
	{	//spawn volumes inactive
		for (size_t i = 0; i < SpawnVolumeActors.Num(); ++i)
		{
			ASpawnVolume* volume = SpawnVolumeActors[i];
			volume->SetSpawningActive(true);
		}
		//block player input
		APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
		if (playerController)
		{
			playerController->SetCinematicMode(true, false, false, true, true);
		}
		//ragdoll the character
		ACharacter* mCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		if (mCharacter)
		{
			mCharacter->GetMesh()->SetSimulatePhysics(true);
			mCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}
		break; 
	}

	case EBatteryPlayState::EWon:
	{
		//spawn volumes inactive
		for (size_t i = 0; i < SpawnVolumeActors.Num(); ++i)
		{
			ASpawnVolume* volume = SpawnVolumeActors[i];
			volume->SetSpawningActive(false);
		}
		break; 
	}

	case EBatteryPlayState::EUnknown:
		//do nothing
		break;

	default:
		//do nothing
		break;
	}
}

