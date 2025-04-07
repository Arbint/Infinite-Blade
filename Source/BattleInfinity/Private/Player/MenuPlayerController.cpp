// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MenuPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMenuPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	if (IsLocalController())
	{
		UUserWidget* MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
}
