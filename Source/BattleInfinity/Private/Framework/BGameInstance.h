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
	virtual void Init() override;
	/***********************************/
	/*            Client               */
	/***********************************/
public:
	FOnLoginCompleted OnLoginCompleted;
	void ClientLogin();
private:
	void LoginCompleted(int32 PlayerNum, bool bWasSuccessful, const FUniqueNetId& NetId, const FString& Error);

	/***********************************/
	/*            Server               */
	/***********************************/
private:	
	void CreateSession();

	FString GetSessionName() const;
	FName GetSesionNameKey() const;

	FString GetSessionUniqueID() const;
	FName GetSesionUniqueIDKey() const;

	void SessionCreated(FName SessionName, bool bWasSuccessfull);

	void LoadLevelAndListen(const TSoftObjectPtr<UWorld>& LevelToLoad);

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSoftObjectPtr<UWorld> MatchLevel;
};
