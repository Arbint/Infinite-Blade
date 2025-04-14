// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/OnlineSessionInterface.h"
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
	void RequestCreateSession(const FName& NewSessionName);
private:
	void LoginCompleted(int32 PlayerNum, bool bWasSuccessful, const FUniqueNetId& NetId, const FString& Error);
	FString GetCoordinatorURLStr();
	FName GetCoordinatorURLStrKey();
	void SessionCreationRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessdSuccessfully, FGuid SessionUniqueId);

	void StartFindCreatedSession(const FGuid& SessionUniqueId, int Port);
	void StopFindingCreatedSession();
	void FindCreatedSession(FGuid SessionUniqueId, int Port);
	void FindCreatedSessionMaxTimeReached();
	void FindCreatedSessionCompleted(bool bWasSuccessful, int Port);
	void JoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type JoinSessionCompletedResult, int Port);

	TSharedPtr<FOnlineSessionSearch> OnlineSessionSearch;
	
	FTimerHandle FindCreatedSesionTimerHandle;
	FTimerHandle FindCreatedSesionTimeoutTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Session Search")
	float FindCreatedSessionInterval = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Session Search")
	float FindCreatedSessionSearchMaxTime = 30.f;

	IOnlineSessionPtr GetOnlineSesionPtr() const;
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
