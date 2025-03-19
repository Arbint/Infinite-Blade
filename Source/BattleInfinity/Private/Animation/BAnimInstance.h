// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "BAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:	
	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool GetIsMoving() const { return Speed > 0.f; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool GetIsNotMoving() const { return Speed == 0.f; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	float GetSmoothedYawSpeed() const { return SmoothedYawSpeed; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool GetIsFalling() const { return bIsFalling; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool GetIsOnGround() const { return !bIsFalling; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	float GetLookYaw() const { return LookRotationDelta.Yaw; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	float GetLookPitch() const { return LookRotationDelta.Pitch; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool GetIsAiming() const { return bIsAiming; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	float GetForwardSpeed() const { return ForwardSpeed; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	float GetRightSpeed() const { return RightSpeed; }

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool ShouldDoFullBody() const;

private:
	void OwnerAimTagUpdated(const FGameplayTag GameplayTag, int32 NewCount);
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* OwnerCharacterMovementComponent;

	float Speed;

	float YawSpeed;
	float SmoothedYawSpeed;
	float ForwardSpeed;
	float RightSpeed;
	bool bIsFalling;
	bool bIsAiming;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float YawSpeedSmoothRate = 7.f;

	FRotator PrevBodyRotation;
	FRotator LookRotationDelta;
};
