// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BPlayerController.h"
#include "Player/BPlayerCharacter.h"

void ABPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	BPlayerCharacter = Cast<ABPlayerCharacter>(NewPawn);
	if (BPlayerCharacter)
	{
		BPlayerCharacter->ServerSideInit();
	}
}

void ABPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	BPlayerCharacter = Cast<ABPlayerCharacter>(NewPawn);
	if (BPlayerCharacter)
	{
		BPlayerCharacter->ClientSideInit();
	}
}
