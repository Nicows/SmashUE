// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Algo/Accumulate.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if(CameraBoundsActor != nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
	}

	InitCameraZoomParameters();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Add(FollowTarget);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if(CameraMain == nullptr) return;
	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();

	//TODO: Find CurrentPercent of distance using:
	// - CalculateGreatestDistanceBetweenTargets
	// - CameraZoomDistanceBetweenTargetsMin
	// - CameraZoomDistanceBetweenTargetsMax
	//Spoiler: it's an InverseLerp but Unreal has no inverseLerp, try to find the name
	//Dont forget to clamp your percent between 0 and 1

	//TODo: Update Main Camera Y position with a lerp using CameraZoomYMin and CameraZoomYMax
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	if(CameraMain == nullptr) return;

	FVector AveragePosition = CalculateAveragePositionBetweenTargets();
	if (AveragePosition.IsZero()) return;

	FVector CurrentCameraPosition = CameraMain->GetOwner()->GetActorLocation();
	AveragePosition.Y = CurrentCameraPosition.Y;
	
	CameraMain->GetOwner()->SetActorLocation(AveragePosition);
}

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	TArray<AActor*> OutCameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraBounds", OutCameras);
	if (OutCameras.Num() > 0)
	{		
		return OutCameras[0];
	}

	return nullptr;
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundCenter;
	FVector BoundsExtents;
	CameraBoundsActor->GetActorBounds(false, BoundCenter, BoundsExtents);

	CameraBoundsMin = FVector2D(BoundCenter.X - BoundsExtents.X, BoundCenter.Y - BoundsExtents.Y);
	CameraBoundsMax = FVector2D(BoundCenter.X + BoundsExtents.X, BoundCenter.Y + BoundsExtents.Y);

	CameraBoundsYProjectionCenter = BoundCenter.Y;
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewPortBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewPortBoundsMin, ViewportBoundsMax);

	FVector WorldBoundsMin = CalculateWorlPositionFromViewportPosition(ViewPortBoundsMin);
	FVector WorldBoundsMax = CalculateWorlPositionFromViewportPosition(ViewportBoundsMax);

	Position = ClampVector(Position, WorldBoundsMin, WorldBoundsMax);
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	//Find Viewport
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr) return;

	//calculate Viewport React according to Camera Aspect Ratio and Viewport ViewRect
	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
	);
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);

	//Fill Output parameters with ViewportRect
	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
	
}

FVector UCameraWorldSubsystem::CalculateWorlPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if(CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;
	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
	);

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{
	//TODO: Find CameraDistanceMin (using tag) and Update CameraZoomYMin according to Y position if found
	
	//TODO: Find CameraDistanceMax (using tag) and Update CameraZoomYMax according to Y position if found
	
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	if(FollowTargets.Num() == 0) return FVector::ZeroVector;

	FVector AveragePosition = FVector::ZeroVector;

	for (UObject* OutTarget : FollowTargets)
	{
		if (AActor* ActorTarget = Cast<AActor>(OutTarget)) 
		{
			AveragePosition += ActorTarget->GetActorLocation();
		}
	}

	return AveragePosition / FollowTargets.Num();
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0.f;

	//TODO: Iterate other FollowerTargets array to calculate greatest distance
	//Spoiler your probably need two loops

	return GreatestDistance;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> OutCameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, OutCameras);
	if (OutCameras.Num() > 0)
	{
		UCameraComponent* MainCameraToGet = OutCameras[0]->FindComponentByClass<UCameraComponent>();
		if(MainCameraToGet == nullptr) return nullptr;
		
		return MainCameraToGet;
	}

	return nullptr;

}

