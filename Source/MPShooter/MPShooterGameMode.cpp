// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPShooterGameMode.h"
#include "MPShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMPShooterGameMode::AMPShooterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
