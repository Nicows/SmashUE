// Fill out your copyright notice in the Description page of Project Settings.

#include "Match/MatchGameMode.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Arena/ArenaPlayerStart.h"
#include "Arena/ArenaSettings.h"
#include "Characters/SmashCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/SmashCharacterSettings.h"

void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		AArenaPlayerStart* ArenaPlayerStartActor = Cast <AArenaPlayerStart>(FoundActors[i]);
		if (ArenaPlayerStartActor == nullptr) continue;

		ResultsActors.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::SpawnCharacter(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	USmashCharacterInputData* InputData = LoadInputDataFromConfig();

	if (SpawnPoints.IsEmpty()) return;
	
	// Associe chaque joueur local à un point de spawn
	for (int32 PlayerIndex = 0; PlayerIndex < SpawnPoints.Num(); PlayerIndex++)
	{
		if (!SpawnPoints.IsValidIndex(PlayerIndex)) continue;

		AArenaPlayerStart* SpawnPoint = SpawnPoints[PlayerIndex];
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();

		// Détermine la classe de personnage à spawn pour ce type d'entrée
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;

		// Spawn le personnage
		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(
			SmashCharacterClass,
			SpawnPoint->GetTransform()
		);

		if (NewCharacter == nullptr) continue;

		// Configure le personnage
		NewCharacter->InputData = InputData;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());

		// Associe le joueur local au personnage
		
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);

		if (PlayerController != nullptr)
		{
			PlayerController->Possess(NewCharacter);
		}
		// else
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("PlayerController %d not found in AMatchGameMode::SpawnCharacter"), PlayerIndex);
		// }

		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
		CharactersInsideArena.Add(NewCharacter);
	}
}


TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	switch (InputType)
	{
		case EAutoReceiveInput::Player0:
			return ArenaSettings->SmashCharacterClassP0;
		
		case EAutoReceiveInput::Player1:
			return ArenaSettings->SmashCharacterClassP1;
		
		case EAutoReceiveInput::Player2:
			return ArenaSettings->SmashCharacterClassP2;
		
		case EAutoReceiveInput::Player3:
			return ArenaSettings->SmashCharacterClassP3;
		
		default:
			return nullptr;
	}
}

void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreateAndInitPlayers();

	TArray<AArenaPlayerStart*> PlayerStartPoints;
	FindPlayerStartActorsInArena(PlayerStartPoints);
	SpawnCharacter(PlayerStartPoints);

	for(AArenaPlayerStart* PlayerStartPoint: PlayerStartPoints)
	{
		EAutoReceiveInput::Type InputType = PlayerStartPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;
		
		// GEngine->AddOnScreenDebugMessage(
		// 	-1,
		// 	3.f,
		// 	FColor::Cyan,
		// 	SmashCharacterClass->GetFName().ToString()
		// );
	}
}

void AMatchGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if(GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}

USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* AMatchGameMode::LoadInputMappingContextFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputMappingContext.LoadSynchronous();
}



