// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
private:	
	UPROPERTY(meta=(BindWidget))
	class UButton* LoginButton;

	UPROPERTY(meta=(BindWidget))
	class UButton* CreateSessionBtn;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* UserNameText;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* SessionNameText;

	UPROPERTY()
	class UBGameInstance* BGameInstance;

	UFUNCTION()
	void LoginButtonClicked();

	UFUNCTION()
	void CreateSesionButtonClicked();

	void LoginCompleted(bool bWasSuccessful, const FString& PlayerName, const FString& Error);
};

