// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/BGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

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
