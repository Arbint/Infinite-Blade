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
			IdentityPtr->Login(0, FOnlineAccountCredentials{"AccountPortal", "", ""});
		}
	}
}
