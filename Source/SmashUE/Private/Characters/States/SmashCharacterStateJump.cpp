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
	Character->GetCharacterMovement()->JumpZVelocity = JumpWalkSpeed;
	Character->JumpKeyHoldTime = 0;
	Character->JumpMaxHoldTime = JumpDuration;
	Character->Jump();
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		2.f,
		FColor::Green,
		FString::Printf(TEXT("Enter State Jump"))
	);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->StopJumping();

	GEngine->AddOnScreenDebugMessage(
		-1,
		2.f,
		FColor::Red,
		FString::Printf(TEXT("Exit State Jump"))
	);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	Character->JumpKeyHoldTime += DeltaTime;
	if(!Character->GetInputJump() || Character->JumpKeyHoldTime > Character->JumpMaxHoldTime)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	
	if(!Character->GetCharacterMovement()->IsFalling())
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	

	if (FMath::Abs(Character->GetInputMoveX()) >= GetDefault<USmashCharacterSettings>()->InputMoveXThreshold)
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector * JumpAirControl, Character->GetOrientX());
	}
		
	
}

void USmashCharacterStateJump::ChangeStateAnim()
{
	Super::ChangeStateAnim();
}
