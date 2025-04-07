// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:	
	void ClientLogin();
};
