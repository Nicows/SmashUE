﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateWalk.generated.h"


UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateWalk : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;
	
	virtual void StateTick(float DeltaTime) override;

	virtual void ChangeStateAnim() override;

	UPROPERTY(EditAnywhere)
	float MoveSpeedMax = 250.f;
	
	UFUNCTION()
	void OnInputMoveXFast(float InputMoveX);
	
	UFUNCTION()
	void OnInputJump(bool InputJump);
};
