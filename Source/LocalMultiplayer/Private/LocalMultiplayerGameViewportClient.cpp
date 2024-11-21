// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "ViewportClient.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if(GameInstance == nullptr)
		return false;
	
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	
	if (LocalMultiplayerSettings == nullptr || LocalMultiplayerSubsystem == nullptr) return false;
	
	int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
	
	int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);

	if(PlayerIndex < 0) return false;
	ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
	if (LocalPlayer == nullptr)
		return false;
	
	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GameInstance->GetWorld());
	if (PlayerController == nullptr)
		return false;
	
	UE_LOG(LogTemp, Display, TEXT("ULocalMultiplayerGameViewportClient::InputKey, InputKey: Key = %s, PlayerIndex = %d, "), *EventArgs.Key.ToString(), PlayerIndex);
	PlayerController->InputKey(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad());
	
	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (GameInstance == nullptr) return false;

	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSettings == nullptr || LocalMultiplayerSubsystem == nullptr) return false;

	int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(Key, ELocalMultiplayerInputMappingType::InGame);
	int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
	if (PlayerIndex < 0) return false;

	ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
	if (LocalPlayer == nullptr) return false;

	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GameInstance->GetWorld());
	if (PlayerController == nullptr) return false;

	PlayerController->InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);

	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}

