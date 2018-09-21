// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float DeltaTime) override;

	/**returns power needed to win*/
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetPowerToWin() const;

	virtual void BeginPlay() override;

protected:
	/**rate at which the character loses power*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float DecayRate;

	/**power needed to win the game*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float PowerToWin;

	/**widget to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	/**instance of the HUD*/
	UPROPERTY()
		class UUserWidget* CurrentWidget;

};



