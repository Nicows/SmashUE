// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CameraSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Smash Camera Settings"))
class SMASHUE_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = CameraSettings)
	FName CameraMainTag;

	UPROPERTY(Config, EditAnywhere, Category = CameraSettings)
	FName CameraDistanceMinTag;

	UPROPERTY(Config, EditAnywhere, Category = CameraSettings)
	FName CameraDistanceMaxTag;

	UPROPERTY(Config, EditAnywhere, Category = CameraSettings)
	FName CameraBoundsTag;

	UPROPERTY(Config, EditAnywhere, Category = CameraSettings)
	float DistanceBetweenTargetsMin;

	UPROPERTY(Config, EditAnywhere, Category = CameraSettings)
	float DistanceBetweenTargetsMax;

	UPROPERTY(Config, EditAnywhere, Category = CameraSettings)
	float SizeDampingFactor;

	UPROPERTY(Config, EditAnywhere, Category = CameraSettings)
	float PositionDampingFactor;
};
