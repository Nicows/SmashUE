﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraFollowTarget.h"
#include "SmashCharacter.generated.h"

class USmashCharacterStateMachine;
class UInputMappingContext;
class USmashCharacterInputData;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter,
									public ICameraFollowTarget
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion
#pragma region Orient

public:
	float GetOrientX() const;

	void SetOrientX(float NewOrientX);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;
	
#pragma endregion
#pragma region State Machine
public:
	void CreateStateMachine();
	
	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
#pragma endregion
#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;

#pragma endregion
#pragma region Input Move X

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputJumpEvent, bool, InputJump);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputAttackEvent, bool, InputAttack);
	
public:
	float GetInputMoveX() const;
	float GetInputJump() const;
	float GetInputSpecialAttack() const;

	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;
	
	UPROPERTY()
	FInputJumpEvent InputJumpEvent;

	UPROPERTY()
	FInputAttackEvent InputSpecialAttackEvent;
	

protected:
	UPROPERTY(EditAnywhere)
	float InputMoveX = 0.f;
	
	UPROPERTY(EditAnywhere)
	bool InputJump = false;
	
	UPROPERTY(EditAnywhere)
	bool InputSpecialAttack = false;

private:
	void BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	
	void OnInputJump(const FInputActionValue& InputActionValue);
	
	void OnInputSpecialAttack(const FInputActionValue& InputActionValue);

#pragma endregion
#pragma region Camera Target
	
public:
	virtual bool IsFollowable() override;
	
	virtual FVector GetFollowPosition() override;

private:
#pragma endregion
};
