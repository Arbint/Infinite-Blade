// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "BCharacter.generated.h"

UCLASS()
class ABCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABCharacter();

	void ServerSideInit();
	void ClientSideInit();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/************************************************/
	/*              Gameplay Ability                */
	/************************************************/
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
private:
	void BindAbilitySystemDelegates();

	UPROPERTY(VisibleDefaultsOnly, Category = "GameplayAbility")
	class UBAbilitySystemComponent* BAbilitySystemComponent;

	UPROPERTY()
	class UBAttributeSet* BAttributeSet;
	/************************************************/
	/*                   Widgets                    */
	/************************************************/
private:
	UPROPERTY(VisibleDefaultsOnly, Category="Widget")
	class UWidgetComponent* OverheadWidgetComponent;

	void ConfigureOverheadWidget();
	/************************************************/
	/*             Death and Respawn                */
	/************************************************/
private:
	void DeadTagUpdated(const FGameplayTag Tag, int32 NewCount);

	void StartDeathSequence();
	void Respawn();

	virtual void OnDead();
	virtual void OnRespawn();

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* DeathMontage;

	void PlayDeathAnimation();
	void SetEnableRagdoll(bool bEnableRagdoll);
	void DeathMontageFinished();

	FTransform CachedMeshRelativeTransform;

	/************************************************/
	/*                     AI                       */
	/************************************************/
private:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) { TeamId = NewTeamID; }
	
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const { return TeamId; }

	UPROPERTY(Replicated)
	FGenericTeamId TeamId;
};
