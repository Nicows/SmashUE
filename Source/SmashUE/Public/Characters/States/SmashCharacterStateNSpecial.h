// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateNSpecial.generated.h"

class ASmashCharacterProjectile;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateNSpecial : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	
	virtual void StateTick(float DeltaTime) override;

	virtual void ChangeStateAnim() override;
	
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AActor> ProjectileClass;

	UPROPERTY()
	ASmashCharacterProjectile* Projectile;
	
	UPROPERTY(EditAnywhere, Category = Projectile)
	double ProjectileMinScale;
	
	UPROPERTY(EditAnywhere, Category = Projectile)
	double ProjectileMaxScale;
};
