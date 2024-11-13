// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateJump.generated.h"


UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	
	virtual void StateTick(float DeltaTime) override;

	virtual void ChangeStateAnim() override;

	UPROPERTY(EditAnywhere)
	float JumpWalkSpeed;

	UPROPERTY(EditAnywhere)
	float JumpMaxHeight;

	UPROPERTY(EditAnywhere)
	float JumpDuration;

	UPROPERTY(EditAnywhere)
	float JumpAirControl;
};
