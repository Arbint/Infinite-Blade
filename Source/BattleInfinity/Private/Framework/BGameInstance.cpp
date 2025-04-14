// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/BGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "HttpModule.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/IHttpResponse.h"

void UBGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Init Called"))
	if (GetWorld()->GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to Create Session"))
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

void UBGameInstance::RequestCreateSession(const FName& NewSessionName)
{
	FGuid NewSesionUniqueId = FGuid::NewGuid(); 
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	
	FString CoordinatorURLStr = GetCoordinatorURLStr();
	FString RequestURL = FString::Printf(TEXT("%s/StartServer"), *(CoordinatorURLStr));
	Request->SetURL(RequestURL);
	Request->SetVerb("POST");

	Request->SetHeader(GetSesionUniqueIDKey().ToString(), NewSesionUniqueId.ToString());
	Request->SetHeader(GetSesionNameKey().ToString(), NewSessionName.ToString());

	Request->OnProcessRequestComplete().BindUObject(this, &UBGameInstance::SessionCreationRequestCompleted, NewSesionUniqueId);
	Request->ProcessRequest();
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

FString UBGameInstance::GetCoordinatorURLStr()
{
	FString CoordiantorURL = "";
	FParse::Value(FCommandLine::Get(), *GetCoordinatorURLStrKey().ToString(), CoordiantorURL);
	UE_LOG(LogTemp, Warning, TEXT("Found Coordinator URL: %s"),*(CoordiantorURL));
	if (CoordiantorURL == "")
		return "127.0.0.1";

	return CoordiantorURL;
}

FName UBGameInstance::GetCoordinatorURLStrKey()
{
	return FName("COORDINATOR_URL");
}

void UBGameInstance::SessionCreationRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bProcessdSuccessfully, FGuid SessionUniqueId)
{
	if (bProcessdSuccessfully)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Created Successfully"));
		FString PortStr = Response->GetHeader("PORT");
		int Port = FCString::Atoi(*PortStr);
		StartFindCreatedSession(SessionUniqueId, Port);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session did not got created..."));
	}
}

void UBGameInstance::StartFindCreatedSession(const FGuid& SessionUniqueId, int Port)
{
	StopFindingCreatedSession();
	GetWorld()->GetTimerManager().SetTimer(FindCreatedSesionTimerHandle, 
		FTimerDelegate::CreateUObject(this, &UBGameInstance::FindCreatedSession, SessionUniqueId, Port),
		FindCreatedSessionInterval, true	
	); // tring to find the created session peoridically

	GetWorld()->GetTimerManager().SetTimer(FindCreatedSesionTimeoutTimerHandle,
		this, &UBGameInstance::FindCreatedSessionMaxTimeReached,
		FindCreatedSessionSearchMaxTime
	); // stop the search if max time has reached.
}

void UBGameInstance::StopFindingCreatedSession()
{
	GetWorld()->GetTimerManager().ClearTimer(FindCreatedSesionTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(FindCreatedSesionTimeoutTimerHandle);
	IOnlineSessionPtr OnlineSessionPtr = GetOnlineSesionPtr();
	if (OnlineSessionPtr)
	{
		OnlineSessionPtr->OnFindSessionsCompleteDelegates.RemoveAll(this);
	}
}

void UBGameInstance::FindCreatedSession(FGuid SessionUniqueId, int Port)
{
	// always do dynamic allocation in it's own line.
	FOnlineSessionSearch* NewSessionSearch = new FOnlineSessionSearch;
	OnlineSessionSearch = MakeShareable(NewSessionSearch);

	if (OnlineSessionSearch)
	{
		OnlineSessionSearch->bIsLanQuery = false;
		OnlineSessionSearch->MaxSearchResults = 100;
		OnlineSessionSearch->QuerySettings.Set(GetSesionUniqueIDKey(), SessionUniqueId.ToString(), EOnlineComparisonOp::Equals);
	}

	IOnlineSessionPtr OnlineSessionPtr = GetOnlineSesionPtr();
	if (OnlineSessionPtr)
	{
		OnlineSessionPtr->OnFindSessionsCompleteDelegates.RemoveAll(this);
		OnlineSessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UBGameInstance::FindCreatedSessionCompleted, Port);
		if (OnlineSessionPtr->FindSessions(0, OnlineSessionSearch.ToSharedRef()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Find Created Session failed when calling IOnlineSessionPtr::FindSessions"));
			OnlineSessionPtr->OnFindSessionsCompleteDelegates.RemoveAll(this);
		}
	}
}

void UBGameInstance::FindCreatedSessionMaxTimeReached()
{
	StopFindingCreatedSession();
	UE_LOG(LogTemp, Warning, TEXT("Can't find create session after %f seconds, abort"), FindCreatedSessionSearchMaxTime);
}

void UBGameInstance::FindCreatedSessionCompleted(bool bWasSuccessful, int Port)
{
	if (!bWasSuccessful || OnlineSessionSearch->SearchResults.Num() == 0)
		return;

	StopFindingCreatedSession();

	FOnlineSessionSearchResult OnlineSessionSearchResult = OnlineSessionSearch->SearchResults[0];
	UE_LOG(LogTemp, Warning, TEXT("Found Created Session with id: %s"), *OnlineSessionSearchResult.GetSessionIdStr());
}

IOnlineSessionPtr UBGameInstance::GetOnlineSesionPtr() const
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return nullptr;
	}
	return OnlineSubsystem->GetSessionInterface();
}

void UBGameInstance::CreateSession()
{

	UE_LOG(LogTemp, Warning, TEXT("Creating Session"))
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
	OnlineSessionSettings.Set(GetSesionUniqueIDKey(), GetSessionUniqueID(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionPtr->OnCreateSessionCompleteDelegates.RemoveAll(this);
	SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UBGameInstance::SessionCreated);
	if (!SessionPtr->CreateSession(0, FName(GetSessionName()), OnlineSessionSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Create Session..."))
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

FString UBGameInstance::GetSessionUniqueID() const
{
	FString UniqueId = "";
	FParse::Value(FCommandLine::Get(), *GetSesionUniqueIDKey().ToString(), UniqueId);
	UE_LOG(LogTemp, Warning, TEXT("Found Unique Id: %s"),*(UniqueId));
	return UniqueId;
}

FName UBGameInstance::GetSesionUniqueIDKey() const
{
	return FName("SESSION_UNIQUE_ID");
}

void UBGameInstance::SessionCreated(FName SessionName, bool bWasSuccessfull)
{
	UE_LOG(LogTemp, Warning, TEXT("Create Session Callback reached"))
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
