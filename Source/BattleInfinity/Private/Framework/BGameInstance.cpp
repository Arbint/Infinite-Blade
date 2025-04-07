// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/BGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"

void UBGameInstance::Init()
{
	Super::Init();
	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		CreateSession();
	}
}

void UBGameInstance::ClientLogin()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		IOnlineIdentityPtr IdentityPtr = OnlineSubsystem->GetIdentityInterface();
		if (IdentityPtr)
		{
			IdentityPtr->OnLoginCompleteDelegates->RemoveAll(this);
			IdentityPtr->OnLoginCompleteDelegates->AddUObject(this, &UBGameInstance::LoginCompleted);
			IdentityPtr->Login(0, FOnlineAccountCredentials{ "AccountPortal", "", "" });
		}
	}
}

void UBGameInstance::LoginCompleted(int32 PlayerNum, bool bWasSuccessful, const FUniqueNetId& NetId, const FString& Error)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return;
	}

	IOnlineIdentityPtr IdentityPtr = OnlineSubsystem->GetIdentityInterface();
	if (!IdentityPtr)
	{
		return;
	}

	if (bWasSuccessful)
	{
		FString PlayerName = IdentityPtr->GetPlayerNickname(NetId);
		OnLoginCompleted.ExecuteIfBound(true, PlayerName, Error);
	}

	IdentityPtr->OnLoginCompleteDelegates->RemoveAll(this);
}

void UBGameInstance::CreateSession()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return;
	}

	IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface();
	if (!SessionPtr)
	{
		return;
	}

	FOnlineSessionSettings OnlineSessionSettings{};
	OnlineSessionSettings.bAllowInvites = true;
	OnlineSessionSettings.bAllowJoinInProgress = false;
	OnlineSessionSettings.bIsDedicated = true;
	OnlineSessionSettings.bIsLANMatch = false;
	OnlineSessionSettings.bShouldAdvertise = true;
	OnlineSessionSettings.NumPublicConnections = 10;

	OnlineSessionSettings.Set(GetSesionNameKey(), GetSessionName(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionPtr->OnCreateSessionCompleteDelegates.RemoveAll(this);
	SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UBGameInstance::SessionCreated);
	if (SessionPtr->CreateSession(0, FName(GetSessionName()), OnlineSessionSettings))
	{
		SessionPtr->OnCreateSessionCompleteDelegates.RemoveAll(this);
	}
}

FString UBGameInstance::GetSessionName() const
{
	return "Test Session Name";
}

FName UBGameInstance::GetSesionNameKey() const
{
	return FName("SESSION_NAME");
}

void UBGameInstance::SessionCreated(FName SessionName, bool bWasSuccessfull)
{
	if (bWasSuccessfull)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Session Successful"));
		LoadLevelAndListen(MatchLevel);
	}
}

void UBGameInstance::LoadLevelAndListen(const TSoftObjectPtr<UWorld>& LevelToLoad)
{
	const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(LevelToLoad.ToString()));
	GetWorld()->ServerTravel(LevelName.ToString() + "?listen");
}
