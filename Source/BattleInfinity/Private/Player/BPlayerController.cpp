// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BPlayerController.h"
#include "Player/BPlayerCharacter.h"
#include "Widgets/GameplayWidget.h"
#include "Net/UnrealNetwork.h"


void ABPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABPlayerController, TeamId);
}

void ABPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamId = NewTeamID;
}

FGenericTeamId ABPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void ABPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	BPlayerCharacter = Cast<ABPlayerCharacter>(NewPawn);
	if (BPlayerCharacter)
	{
		BPlayerCharacter->ServerSideInit();
		BPlayerCharacter->SetGenericTeamId(GetGenericTeamId());
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
	CreateGameplayWidget();
}

void ABPlayerController::CreateGameplayWidget()
{
	if (!IsLocalPlayerController())
		return;

	if (!GameplayWidgetClass)
		return;

	GameplayWidget = CreateWidget<UGameplayWidget>(this, GameplayWidgetClass);
	if (GameplayWidget)
	{
		GameplayWidget->AddToViewport();
	}
}
