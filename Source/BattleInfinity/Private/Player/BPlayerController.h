// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "BPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ABPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:	
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

	//Calls on the server only
	virtual void OnPossess(APawn* NewPawn) override;

	//calls on the clients and listening server only
	virtual void AcknowledgePossession(APawn* NewPawn) override;

private:
	UPROPERTY(Replicated)
	FGenericTeamId TeamId;

	UPROPERTY()
	class ABPlayerCharacter* BPlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UGameplayWidget> GameplayWidgetClass;

	UPROPERTY()
	class UGameplayWidget* GameplayWidget;

	void CreateGameplayWidget();
};
