// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmashCharacterProjectile.generated.h"

UCLASS()
class SMASHUE_API ASmashCharacterProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmashCharacterProjectile();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ProjectileSpeed = 10.f;
	float ProjectileOrient;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ProjectileDamageAmount = 0.f;

	float CurrentProjectileLifeTime = 0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ProjectileLifeTime = 20.f;

	bool bIsMoving = false;
	
	UFUNCTION(BlueprintCallable)
	void ShootProjectile();
	
	UFUNCTION()
	float GetProjectileSpeed() const;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* CollisionBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
