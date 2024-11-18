// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraFollowTarget.h"
#include "Camera/CameraSettings.h"
#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(GetDefault<UCameraSettings>()->CameraMainTag);

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

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	if(CameraMain == nullptr) return;

	FVector AveragePosition = CalculateAveragePositionBetweenTargets();
	if (AveragePosition.IsZero()) return;

	FVector CurrentCameraPosition = CameraMain->GetOwner()->GetActorLocation();
	FVector NewPosition = FVector(AveragePosition.X, CurrentCameraPosition.Y, AveragePosition.Z);
	
	ClampPositionIntoCameraBounds(NewPosition);

	FVector SmoothedPosition = FMath::Lerp(CurrentCameraPosition, NewPosition, DeltaTime * GetDefault<UCameraSettings>()->PositionDampingFactor);

	CameraMain->GetOwner()->SetActorLocation(SmoothedPosition);
}

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	TArray<AActor*> OutCameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), GetDefault<UCameraSettings>()->CameraBoundsTag, OutCameras);
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

	CameraBoundsMin = FVector2D(BoundCenter.X - BoundsExtents.X, BoundCenter.Z - BoundsExtents.Z);
	CameraBoundsMax = FVector2D(BoundCenter.X + BoundsExtents.X, BoundCenter.Z + BoundsExtents.Z);
	
	CameraBoundsYProjectionCenter = BoundCenter.Y;

}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewPortBoundsMin, ViewPortBoundsMax;
	GetViewportBounds(ViewPortBoundsMin, ViewPortBoundsMax);

	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewPortBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewPortBoundsMax);
	
	Position.X = FMath::Clamp(
	Position.X,
	CameraBoundsMin.X - WorldBoundsMin.X / 2,
	CameraBoundsMax.X - WorldBoundsMax.X / 2
	);

	Position.Z = FMath::Clamp(
		Position.Z,
		CameraBoundsMin.Y - WorldBoundsMax.Z / 2,
		CameraBoundsMax.Y - WorldBoundsMin.Z / 2
	);
	
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	//Find Viewport
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ViewportClient is nullptr"));
		return;
	}
	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Viewport inside ViewportClient is nullptr"));
		return;
	}

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

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if(CameraMain == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraMain is nullptr"));
		return FVector::ZeroVector;
	}

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
	UCameraComponent* CameraDistanceMin = FindCameraByTag(GetDefault<UCameraSettings>()->CameraDistanceMinTag);
	if(CameraDistanceMin != nullptr) 
		CameraZoomYMin = CameraDistanceMin->GetComponentLocation().Y;
	else
		UE_LOG(LogTemp, Warning, TEXT("CameraDistanceMin not found"));
	
	UCameraComponent* CameraDistanceMax = FindCameraByTag(GetDefault<UCameraSettings>()->CameraDistanceMaxTag);
	if(CameraDistanceMax != nullptr)
		CameraZoomYMax = CameraDistanceMax->GetComponentLocation().Y;
	else
		UE_LOG(LogTemp, Warning, TEXT("CameraDistanceMax not found"));

}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if(CameraMain == nullptr) return;
	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();


	// Trouver le pourcentage actuel en fonction des distances min et max
	float CurrentPercent = FMath::GetMappedRangeValueClamped(
		FVector2D(CameraZoomDistanceBetweenTargetsMax, CameraZoomDistanceBetweenTargetsMin),
		FVector2D(0.0f, 1.0f),
		GreatestDistanceBetweenTargets
	);

	// Interpoler la position Y de la caméra en fonction du pourcentage
	float NewCameraY = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, CurrentPercent);

	// Obtenir la position actuelle de la caméra et mettre à jour uniquement l'axe Y
	FVector CurrentCameraPosition = CameraMain->GetOwner()->GetActorLocation();
	FVector NewCameraPosition = FVector(CurrentCameraPosition.X, NewCameraY, CurrentCameraPosition.Z);

	FVector SmoothedPosition = FMath::Lerp(CurrentCameraPosition, NewCameraPosition, DeltaTime * GetDefault<UCameraSettings>()->SizeDampingFactor);
	// Appliquer la nouvelle position à la caméra
	CameraMain->GetOwner()->SetActorLocation(SmoothedPosition);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	if(FollowTargets.Num() == 0) return FVector::ZeroVector;

	FVector AveragePosition = FVector::ZeroVector;

	for (UObject* OutTarget : FollowTargets)
	{
		if (ICameraFollowTarget* FollowTarget = Cast<ICameraFollowTarget>(OutTarget)) 
		{
			if(FollowTarget->IsFollowable())
				AveragePosition += FollowTarget->GetFollowPosition();
		}
	}

	return AveragePosition / FollowTargets.Num();
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0.f;

	// Parcourir toutes les cibles de suivi
	for (int32 i = 0; i < FollowTargets.Num(); ++i)
	{
		if (ICameraFollowTarget* TargetA = Cast<ICameraFollowTarget>(FollowTargets[i]))
		{
			if (!TargetA->IsFollowable()) continue;

			FVector PositionA = TargetA->GetFollowPosition();

			// Comparer avec toutes les autres cibles
			for (int32 j = i + 1; j < FollowTargets.Num(); ++j)
			{
				if (ICameraFollowTarget* TargetB = Cast<ICameraFollowTarget>(FollowTargets[j]))
				{
					if (!TargetB->IsFollowable()) continue;

					FVector PositionB = TargetB->GetFollowPosition();

					// Calculer la distance entre les deux cibles
					float Distance = FVector::Dist(PositionA, PositionB);

					// Mettre à jour la plus grande distance trouvée
					GreatestDistance = FMath::Max(GreatestDistance, Distance);
				}
			}
		}
	}
	// Clamp distance min and max using GetDefault<UCameraSettings>()
	GreatestDistance = FMath::Clamp(
	GreatestDistance,
	GetDefault<UCameraSettings>()->DistanceBetweenTargetsMin,
	GetDefault<UCameraSettings>()->DistanceBetweenTargetsMax
	);

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

