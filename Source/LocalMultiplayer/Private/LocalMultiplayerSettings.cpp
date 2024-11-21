// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"

#include "InputMappingContext.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	return GetIMCFromType(MappingType)->GetMappings().ContainsByPredicate([Key](const FEnhancedActionKeyMapping& InKeyMapping)
	{ 
		return InKeyMapping.Key == Key;
	});
	
	TArray<FEnhancedActionKeyMapping> Mapping = GetIMCFromType(MappingType)->GetMappings();
	for (int32 i = 0; i < Mapping.Num(); i++)
	{
		if(Mapping[i].Key == Key)
			return true;
	}
	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
		case ELocalMultiplayerInputMappingType::Menu:
			return IMCMenu;
		case ELocalMultiplayerInputMappingType::InGame:
			return IMCInGame;
		default:
				return nullptr;
	}
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < KeyboardProfilesData.Num(); i++)
	{
		if (KeyboardProfilesData[i].ContainsKey(Key, MappingType))
			return i;
	}

	return -1;
}
