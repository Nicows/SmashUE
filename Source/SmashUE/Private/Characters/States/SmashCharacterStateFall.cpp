// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateFall.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	ChangeStateAnim();
	
	Character->GetCharacterMovement()->AirControl = FallAirControl;
	Character->GetCharacterMovement()->GravityScale = FallGravityScale;

	GEngine->AddOnScreenDebugMessage(
		-1,
		2.f,
		FColor::Green,
		FString::Printf(TEXT("Enter State Fall"))
	);
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->GetCharacterMovement()->GravityScale = 1;
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if(!Character->GetCharacterMovement()->IsFalling())
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	
	if (FMath::Abs(Character->GetInputMoveX()) >= GetDefault<USmashCharacterSettings>()->InputMoveXThreshold)
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

void USmashCharacterStateFall::ChangeStateAnim()
{
	Super::ChangeStateAnim();
}
