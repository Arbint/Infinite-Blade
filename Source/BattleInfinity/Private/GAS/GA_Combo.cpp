// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA_Combo.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "GameplayTagsManager.h"

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		UAbilityTask_PlayMontageAndWait* PlayComboMontageTask = 
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ComboMontage);

		PlayComboMontageTask->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
		PlayComboMontageTask->ReadyForActivation();

		UAbilityTask_WaitGameplayEvent* WaitComboChangeEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			GetComboChangeTag(), nullptr, false, false);

		WaitComboChangeEvent->EventReceived.AddDynamic(this, &UGA_Combo::HandleComboChange);
		WaitComboChangeEvent->ReadyForActivation();
	}
	NextComboName = NAME_None;
	BindInputPressDelegate();

	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitDamageEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			GetComboTargetReceivedTag()
			);

		WaitDamageEvent->EventReceived.AddDynamic(this, &UGA_Combo::HandleDamageEvent);
		WaitDamageEvent->ReadyForActivation();
	}
}

FGameplayTag UGA_Combo::GetComboChangeTag()
{
	return FGameplayTag::RequestGameplayTag("ability.combo.change");
}

FGameplayTag UGA_Combo::GetComboEndTag()
{
	return FGameplayTag::RequestGameplayTag("ability.combo.change.end");
}

FGameplayTag UGA_Combo::GetComboTargetReceivedTag()
{
	return FGameplayTag::RequestGameplayTag("ability.combo.damage");
}

void UGA_Combo::BindInputPressDelegate()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);

	WaitInputPress->OnPress.AddDynamic(this, &UGA_Combo::HandleComboInputPress);
	WaitInputPress->ReadyForActivation();
}

void UGA_Combo::HandleComboInputPress(float TimeWaited)
{
	BindInputPressDelegate();
	TryCommitNextCombo();
}

void UGA_Combo::TryCommitNextCombo()
{
	if (NextComboName == NAME_None)
		return;

	UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance();
	if (!OwnerAnimInstance)
		return;

	OwnerAnimInstance->Montage_SetNextSection(
		OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage),
		NextComboName,
		ComboMontage
	);
}

void UGA_Combo::HandleComboChange(FGameplayEventData Payload)
{
	FGameplayTag EventTag = Payload.EventTag;
	
	if (EventTag == GetComboEndTag())
	{
		NextComboName = NAME_None;
		return;
	}

	TArray<FName> EventTagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, EventTagNames);
	NextComboName = EventTagNames.Last();
}

void UGA_Combo::HandleDamageEvent(FGameplayEventData Payload)
{
	if (K2_HasAuthority())
	{
		FName CurrentComboName = GetOwnerAnimInstance()->Montage_GetCurrentSection(ComboMontage);
		TSubclassOf<UGameplayEffect> DamageEffectClass = DefaultDamageEffect;

		TSubclassOf<UGameplayEffect>* GameplayEffectClassPtr = ComboEffectsMap.Find(CurrentComboName);
		if (GameplayEffectClassPtr)
			DamageEffectClass = *GameplayEffectClassPtr;

		FGameplayEffectSpecHandle DamageEffectSpecHandle =
			MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel(CurrentSpecHandle, CurrentActorInfo));
		K2_ApplyGameplayEffectSpecToTarget(DamageEffectSpecHandle, Payload.TargetData);
	}
}
