// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateRun.h"
#include "Characters/SmashCharacter.h"

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	ChangeStateAnim();

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateRun")
	);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateRun")
	);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	MoveCharacter(FVector::ForwardVector);
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Green,
		TEXT("Tick StateRun")
	);
}

void USmashCharacterStateRun::ChangeStateAnim()
{
	Super::ChangeStateAnim();
}

void USmashCharacterStateRun::MoveCharacter(FVector InVector)
{
	FVector Direction = FVector(Character->GetOrientX(), 0.0f, 0.0f);
	Character->AddMovementInput(InVector * Direction * MoveSpeedMax);
}
