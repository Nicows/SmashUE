﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Arena/ArenaPlayerStart.h"
#include "Match/MatchGameMode.h"

#include "Kismet/GameplayStatics.h"

void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast <AArenaPlayerStart>(FoundActors[i]);
		if (ArenaPlayerStartActor == nullptr) continue;

		ResultsActors.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AArenaPlayerStart*> PlayerStartPoints;
	FindPlayerStartActorsInArena(PlayerStartPoints);

	for(AArenaPlayerStart* PlayerStartPoint: PlayerStartPoints)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Cyan,
			PlayerStartPoint->GetFName().ToString()
		);
	}
}

