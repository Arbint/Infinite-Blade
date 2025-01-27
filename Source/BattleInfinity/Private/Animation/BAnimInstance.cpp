// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
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
		
		YawSpeed = RotationDelta.Yaw / DeltaSeconds;

		SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, YawSpeedSmoothRate);
	}
}
