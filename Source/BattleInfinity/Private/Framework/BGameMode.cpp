// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/BGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

APlayerController* ABGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* NewPlayerController = Super::SpawnPlayerController(InRemoteRole, Options);
	FGenericTeamId NewPlayerTeamId = GetTeamIdForController(NewPlayerController);
	IGenericTeamAgentInterface* NewPlayerTeamInterface = Cast<IGenericTeamAgentInterface>(NewPlayerController);
	if (NewPlayerTeamInterface)
	{
		NewPlayerTeamInterface->SetGenericTeamId(NewPlayerTeamId);
	}

	NewPlayerController->StartSpot = GetNextStartSpotForTeam(NewPlayerTeamId);
	return NewPlayerController;
}

FGenericTeamId ABGameMode::GetTeamIdForController(const AController* NewController) const
{
	static int NextTeamId = 0;
	return FGenericTeamId((++NextTeamId)%2);
}

AActor* ABGameMode::GetNextStartSpotForTeam(const FGenericTeamId& TeamId) const
{
	const FName* TeamStartSpotTagName = TeamPlayerStartTagMap.Find(TeamId);
	if (!TeamStartSpotTagName)
		return nullptr;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->PlayerStartTag == *TeamStartSpotTagName)
		{
			It->PlayerStartTag = "Used";
			return *It;
		}
	}

	return nullptr;
}
