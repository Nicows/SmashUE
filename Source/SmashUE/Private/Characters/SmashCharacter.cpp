// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/SmashCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "LocalMultiplayerGameViewportClient.h"
#include "Camera/CameraWorldSubsystem.h"

#include "Characters/SmashCharacterStateMachine.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASmashCharacter::ASmashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	InitStateMachine();

	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->AddFollowTarget(this);
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputMoveXAxisAndActions(EnhancedInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	// Rotation.Yaw = -90.f * OrientX;
	Rotation.Yaw = OrientX < 0 ? 90.f : -90.f ;
	
	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if (StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if(StateMachine == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("State machine not found %s"), *this->GetClass()->GetName());
		return;
	}
	StateMachine->Tick(DeltaTime);
	
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	return; //unused
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found"));
		return;
	}
	
	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr)
		{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController->GetLocalPlayer not found"));
		return;
	}
	   
	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}

float ASmashCharacter::GetInputJump() const
{
	return InputJump;
}

void ASmashCharacter::BindInputMoveXAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputData == nullptr) return;

	if (InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveX
		);
		
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveX
		);
		
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveX
		);
		
	}

	if(InputData->InputActionMoveXFast)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveXFast,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputMoveXFast
		);
	}

	
	if(InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Triggered,
			this,
			&ASmashCharacter::OnInputJump
		);
	}

	if(InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionJump,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputJump
		);
	}
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}

void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
{
	InputJump = InputActionValue.Get<bool>();
	InputJumpEvent.Broadcast(InputJump);
}

bool ASmashCharacter::IsFollowable()
{
	return true;
}

FVector ASmashCharacter::GetFollowPosition()
{
	return GetActorLocation();
}

