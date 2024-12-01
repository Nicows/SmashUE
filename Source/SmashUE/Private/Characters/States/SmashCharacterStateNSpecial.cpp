// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateNSpecial.h"

#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterProjectile.h"
#include "Characters/SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCharacterStateNSpecial::GetStateID()
{
	return ESmashCharacterStateID::NeutralSpecial;
}

void USmashCharacterStateNSpecial::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	ChangeStateAnim();

	FVector SpawnPosition = Character->GetActorLocation();
	SpawnPosition += FVector::ForwardVector * 150.f * Character->GetOrientX();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character;
	SpawnParams.Instigator = Character->GetInstigator();

	Projectile = GetWorld()->SpawnActor<ASmashCharacterProjectile>(ProjectileClass, SpawnPosition, Character->GetActorRotation(), SpawnParams);
	Projectile->SetActorScale3D(FVector(ProjectileMinScale));
}

void USmashCharacterStateNSpecial::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
}

void USmashCharacterStateNSpecial::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if (Projectile->GetActorScale3D().X < ProjectileMaxScale) 
	{
		FVector CurrentScale = Projectile->GetActorRelativeScale3D();
		FVector TargetScale(ProjectileMaxScale, ProjectileMaxScale, ProjectileMaxScale);
		float LerpAlpha = DeltaTime;

		FVector NewScale = FMath::Lerp(CurrentScale, TargetScale, LerpAlpha);
		Projectile->SetActorScale3D(NewScale);
	}
	
	if(!Character->GetInputSpecialAttack())
	{
		Projectile->ShootProjectile();
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}

void USmashCharacterStateNSpecial::ChangeStateAnim()
{
	Super::ChangeStateAnim();
	if(AnimState == nullptr) return;
	Character->PlayAnimMontage(AnimState);
}
