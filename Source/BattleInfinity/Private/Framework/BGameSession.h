// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "BGameSession.generated.h"

/**
 * 
 */
UCLASS()
class ABGameSession : public AGameSession
{
	GENERATED_BODY()
public:	
	virtual bool ProcessAutoLogin() override;
};
