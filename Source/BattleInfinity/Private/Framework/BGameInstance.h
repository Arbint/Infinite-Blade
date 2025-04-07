// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BGameInstance.generated.h"

DECLARE_DELEGATE_ThreeParams(FOnLoginCompleted, bool/*bWasSuccessful*/, const FString& /*UserName*/, const FString& /*ErrorMsg*/);
/**
 * 
 */
UCLASS()
class UBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:	
	FOnLoginCompleted OnLoginCompleted;
	void ClientLogin();

	void LoginCompleted(int32 PlayerNum, bool bWasSuccessful, const FUniqueNetId& NetId, const FString& Error);
};
