// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateDoubleJump.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateDoubleJump::GetStateID()
{
	return ESmashCharacterStateID::DoubleJump;
}

void USmashCharacterStateDoubleJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	ChangeStateAnim();

	Character->GetCharacterMovement()->JumpZVelocity = (2 * JumpMaxHeight) / (JumpDuration / 2);
	if(CanJumpBackwards())
		Character->GetCharacterMovement()->AddImpulse(FVector::BackwardVector * JumpBackwardsForce, true);

	Character->JumpMaxHoldTime = JumpDuration;
	Character->Jump();
}

void USmashCharacterStateDoubleJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->StopJumping();
}

void USmashCharacterStateDoubleJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if(Character->JumpForceTimeRemaining <= 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	if (FMath::Abs(Character->GetInputMoveX()) >= GetDefault<USmashCharacterSettings>()->InputMoveXThreshold)
	{
		Character->AddMovementInput(FVector::ForwardVector * Character->GetInputMoveX());
	}
}

void USmashCharacterStateDoubleJump::ChangeStateAnim()
{
	Super::ChangeStateAnim();
	if(AnimState == nullptr || AnimBackwardJump == nullptr) return;

	if(CanJumpBackwards())
		Character->PlayAnimMontage(AnimBackwardJump);
	else
		Character->PlayAnimMontage(AnimState);
}

bool USmashCharacterStateDoubleJump::CanJumpBackwards() const
{
	return (FMathf::Round(Character->GetInputMoveX()) != FMathf::Round(Character->GetOrientX())) && Character->GetInputMoveX() != 0;
}
