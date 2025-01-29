// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BCharacter.h"
#include "Camera/CameraComponent.h"
#include "GAS/BAbilitySystemComponent.h"
#include "GAS/BAttributeSet.h"

// Sets default values
ABCharacter::ABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* ABCharacter::GetAbilitySystemComponent() const
{
	return BAbilitySystemComponent;
}

