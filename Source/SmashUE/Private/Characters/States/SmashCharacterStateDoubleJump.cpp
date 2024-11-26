// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateDoubleJump.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateDoubleJump::GetStateID()
{
	return ESmashCharacterStateID::DoubleJump;
}

void USmashCharacterStateDoubleJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	UE_LOG(LogTemp, Display, TEXT("USmashCharacterStateDoubleJump::StateEnter"));
	Super::StateEnter(PreviousStateID);
	ChangeStateAnim();

	Character->GetCharacterMovement()->JumpZVelocity = (2 * JumpMaxHeight) / (JumpDuration / 2);
	Character->Jump();
	// if(Character->GetInputMoveX() <= Character->GetVelocity().X)
	// 	Character->GetCharacterMovement()->AddImpulse(FVector::BackwardVector * Character->GetCharacterMovement()->JumpZVelocity, true);
}

void USmashCharacterStateDoubleJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->StopJumping();
}

void USmashCharacterStateDoubleJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if(Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
}

void USmashCharacterStateDoubleJump::ChangeStateAnim()
{
	Super::ChangeStateAnim();
	if(AnimState == nullptr) return;

	if(Character->GetInputMoveX() > Character->GetOrientX())
		Character->PlayAnimMontage(AnimState);
	else
		Character->PlayAnimMontage(AnimBackwardJump);
}
