// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BAnimInstance.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/BAbilitySystemStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UBAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}

	if (UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TryGetPawnOwner()))
	{
		OwnerASC->RegisterGameplayTagEvent(UBAbilitySystemStatics::GetAimingStatTag()).AddUObject(
			this, &UBAnimInstance::OwnerAimTagUpdated);
	}
}

void UBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter)
	{
		Speed = OwnerCharacter->GetVelocity().Length();

		FRotator BodyRotation = OwnerCharacter->GetActorRotation();
		FRotator RotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(BodyRotation, PrevBodyRotation);
		
		PrevBodyRotation = BodyRotation;
		YawSpeed = RotationDelta.Yaw / DeltaSeconds;

		SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, YawSpeedSmoothRate);

		FRotator AimRotation = OwnerCharacter->GetBaseAimRotation();
		LookRotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, BodyRotation);
	}

	if (OwnerCharacterMovementComponent)
	{
		bIsFalling = OwnerCharacterMovementComponent->IsFalling();
	}
}

void UBAnimInstance::OwnerAimTagUpdated(const FGameplayTag GameplayTag, int32 NewCount)
{
	bIsAiming = NewCount != 0;
}
