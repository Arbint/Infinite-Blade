// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GenericTeamAgentInterface.h"
#include "BGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ABGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:	
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;

private:
	FGenericTeamId GetTeamIdForController(const AController* NewController) const;

	AActor* GetNextStartSpotForTeam(const FGenericTeamId& TeamId) const;

	UPROPERTY(EditDefaultsOnly, Category = "PlayerStart")
	TMap<FGenericTeamId, FName> TeamPlayerStartTagMap;
};
