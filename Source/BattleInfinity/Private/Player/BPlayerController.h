// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ABPlayerController : public APlayerController
{
	GENERATED_BODY()
public:	
	//Calls on the server only
	virtual void OnPossess(APawn* NewPawn) override;

	//calls on the clients and listening server only
	virtual void AcknowledgePossession(APawn* NewPawn) override;


private:
	UPROPERTY()
	class ABPlayerCharacter* BPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

	UPROPERTY()
	class UGameplayWidget* GameplayWidget;

	void CreateGameplayWidget();
};
