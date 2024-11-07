﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateWalk.h"

#include "Characters/SmashCharacter.h"

ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	ChangeStateAnim();

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateWalk")
	);
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateWalk")
	);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	MoveCharacter(FVector::ForwardVector);
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Tick StateWalk")
	);
}

void USmashCharacterStateWalk::ChangeStateAnim()
{
	Super::ChangeStateAnim();
}

void USmashCharacterStateWalk::MoveCharacter(FVector InVector)
{
	FVector Direction = FVector(Character->GetOrientX(), 0.0f, 0.0f);
	Character->AddMovementInput(InVector * Direction * MoveSpeedMax);
}
