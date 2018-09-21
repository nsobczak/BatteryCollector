// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h" //to be able to access the character

#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"

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

	//set the score to beat
	ABatteryCollectorCharacter* mCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (mCharacter)
	{
		PowerToWin = (mCharacter->GetInitialPower())*1.25f;
	}

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
	if (mCharacter && mCharacter->GetCurrentPower() > 0)
	{
		mCharacter->UpdatePower(-DeltaTime * DecayRate *(mCharacter->GetInitialPower()));
	}
}

float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}