// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(AActor* FollowTarget)
{
	
}

void UCameraWorldSubsystem::RemoveFollowTarget(AActor* FollowTarget)
{
	
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	return FVector::ZeroVector;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	return nullptr;
}
