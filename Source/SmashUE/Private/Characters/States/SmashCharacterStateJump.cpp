// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	
	ChangeStateAnim();
	Character->GetCharacterMovement()->AirControl = JumpAirControl;
	Character->GetCharacterMovement()->JumpZVelocity = (2 * JumpMaxHeight) / (JumpDuration / 2);
	Character->GetCharacterMovement()->MaxWalkSpeed = JumpWalkSpeed;
	Character->GetCharacterMovement()->GravityScale = GravityScale;
	
	
	Character->JumpMaxHoldTime = JumpDuration;
	Character->JumpMaxCount = MaxJumpCount;
	Character->JumpCurrentCount = 0;
	Character->Jump();
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->StopJumping();
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if(!Character->GetInputJump() || Character->JumpForceTimeRemaining <= 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}

	if (FMath::Abs(Character->GetInputMoveX()) >= GetDefault<USmashCharacterSettings>()->InputMoveXThreshold)
	{
		// Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
		
	
}

void USmashCharacterStateJump::ChangeStateAnim()
{
	Super::ChangeStateAnim();
	if(AnimState == nullptr) return;
	
	Character->PlayAnimMontage(AnimState);
}
