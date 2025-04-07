// Fill out your copyright notice in the Description page of Project Settings.

#include "Widgets/MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Framework/BGameInstance.h"
#include "MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BGameInstance = GetGameInstance<UBGameInstance>();
	if (BGameInstance)
	{
			
	}
	
	LoginButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LoginButtonClicked);
}

void UMainMenuWidget::LoginButtonClicked()
{
	BGameInstance->ClientLogin();
}
