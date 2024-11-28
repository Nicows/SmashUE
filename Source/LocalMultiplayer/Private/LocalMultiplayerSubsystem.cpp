// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	UE_LOG(LogTemp, Log, TEXT("ULocalMultiplayerSubsystem::CreateAndInitPlayers"));
	
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings)
	{
		UE_LOG(LogTemp, Warning, TEXT("LocalMultiplayerSettings not found"));
		return;
	}

	int PlayerIndex = AssignNewPlayerToKeyboardProfile(0);

	AssignKeyboardMapping(PlayerIndex, 0, MappingType);
	
	// for (int KeyboardProfileIndex = 0; KeyboardProfileIndex < Settings->KeyboardProfilesData.Num(); KeyboardProfileIndex++)
	// {
	// 	int PlayerIndex = AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
	//
	// 	AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, MappingType);
	// }
	//
	// for (int GamepadIndex = 0; GamepadIndex < Settings->NbMaxGamePads; GamepadIndex++)
	// {
	// 	LastAssignedPlayerIndex = 1;
	// 	int PlayerIndex = AssignNewPlayerToGamepadDeviceID(GamepadIndex);
	//
	// 	if(PlayerIndex == -1)
	// 		continue;
	// 	
	// 	AssignGamepadInputMapping(PlayerIndex, MappingType);
	// }
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if (PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
	}

	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	
	UE_LOG(LogTemp, Display, TEXT("ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile"));
	if (PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
	}

	int NewPlayerIndex = LastAssignedPlayerIndex;
	LastAssignedPlayerIndex++;

	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, NewPlayerIndex);

	return NewPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	
	UE_LOG(LogTemp, Display, TEXT("ULocalMultiplayerSubsystem::AssignKeyboardMapping"));
    if (!PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("No PlayerController associated with KeyboardProfileIndex %d"), KeyboardProfileIndex);
        return;
    }

    if (PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex] != PlayerIndex)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerIndex mismatch for KeyboardProfileIndex %d"), KeyboardProfileIndex);
        return;
    }

    const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
    if (!Settings)
    {
        UE_LOG(LogTemp, Warning, TEXT("LocalMultiplayerSettings not found"));
        return;
    }

    if (!Settings->KeyboardProfilesData.IsValidIndex(KeyboardProfileIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid KeyboardProfileIndex: %d"), KeyboardProfileIndex);
        return;
    }

    const FLocalMultiplayerProfileData& ProfileData = Settings->KeyboardProfilesData[KeyboardProfileIndex];

    UInputMappingContext* IMC = ProfileData.GetIMCFromType(MappingType);
    if (!IMC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context not found for MappingType"));
        return;
    }

	UGameInstance* GameInstance = GetGameInstance();
	ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);

	if(LocalPlayer == nullptr)
	{
		FString OutError;
		LocalPlayer = GameInstance->CreateLocalPlayer(PlayerIndex,  OutError, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("GetGameInstance()->GetNumLocalPlayers %d"), GameInstance->GetNumLocalPlayers());
	
    APlayerController* PlayerController = LocalPlayer->GetPlayerController(GameInstance->GetWorld());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found for PlayerIndex %d"), PlayerIndex);
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (InputSubsystem == nullptr)
    {
	    UE_LOG(LogTemp, Warning, TEXT("InputSubsystem not found for PlayerController"));
    	return;
    }

	InputSubsystem->AddMappingContext(IMC, 1);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}

	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}
	if(LastAssignedPlayerIndex == GetDefault<ULocalMultiplayerSettings>()->NbMaxGamePads)
		return -1;

	int NewPlayerIndex = LastAssignedPlayerIndex;

	LastAssignedPlayerIndex++;

	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, NewPlayerIndex);

	return NewPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	UE_LOG(LogTemp, Display, TEXT("ULocalMultiplayerSubsystem::AssignGamepadInputMapping"));
    const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
    if (Settings == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("LocalMultiplayerSettings not found"));
        return;
    }

    const FLocalMultiplayerProfileData& ProfileData = Settings->GamepadProfileData;

    UInputMappingContext* IMC = ProfileData.GetIMCFromType(MappingType);
    if (!IMC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input Mapping Context not found for MappingType"));
        return;
    }

	UGameInstance* GameInstance = GetGameInstance();
	ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);

	if(LocalPlayer == nullptr)
	{
		FString OutError;
		LocalPlayer = GameInstance->CreateLocalPlayer(PlayerIndex,  OutError, true);
	}
	
    APlayerController* PlayerController = LocalPlayer->GetPlayerController(GameInstance->GetWorld());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerController not found for PlayerIndex %d"), PlayerIndex);
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (InputSubsystem == nullptr)
    {
	    UE_LOG(LogTemp, Warning, TEXT("InputSubsystem not found for PlayerController"));
    	return;
    }

	InputSubsystem->AddMappingContext(IMC, 1);
}
