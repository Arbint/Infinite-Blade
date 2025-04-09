// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Framework/BGameInstance.h"
#include "MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BGameInstance = GetGameInstance<UBGameInstance>();
	if (BGameInstance)
	{
		BGameInstance->OnLoginCompleted.BindUObject(this, &UMainMenuWidget::LoginCompleted);
	}
	
	LoginButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LoginButtonClicked);
	CreateSessionBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::CreateSesionButtonClicked);
}

void UMainMenuWidget::LoginButtonClicked()
{
	BGameInstance->ClientLogin();
}

void UMainMenuWidget::CreateSesionButtonClicked()
{
	BGameInstance->RequestCreateSession(FName{SessionNameText->GetText().ToString()});
}

void UMainMenuWidget::LoginCompleted(bool bWasSuccessful, const FString& PlayerName, const FString& Error)
{
	if (bWasSuccessful)
	{
		UserNameText->SetText(FText::FromString(PlayerName));
	}
}
