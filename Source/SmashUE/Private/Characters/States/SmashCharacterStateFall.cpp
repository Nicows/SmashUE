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
	Character->GetCharacterMovement()->MaxWalkSpeed = FallHorizontalMoveSpeed;
	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateFall::OnInputJump);
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	Character->GetCharacterMovement()->GravityScale = 1;
	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateFall::OnInputJump);
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
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
	}
	
}

void USmashCharacterStateFall::ChangeStateAnim()
{
	Super::ChangeStateAnim();
	if(AnimState == nullptr) return;
		Character->PlayAnimMontage(AnimState);
}

void USmashCharacterStateFall::OnInputJump(bool InputJump)
{
	UE_LOG(LogTemp, Display, TEXT("USmashCharacterStateFall::OnInputJump: JumpCurrentCount %d, JumpMaxCount %d, InputJump %d"), Character->JumpCurrentCount, Character->JumpMaxCount, InputJump);
	if(Character->JumpCurrentCount < Character->JumpMaxCount && InputJump == true)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::DoubleJump);
	}
}
