// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN_TraceTarget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/EngineTypes.h"

void UAN_TraceTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);	
	
	if (!MeshComp || !MeshComp->GetOwner())
		return;

	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner()))
		return;

	if (TargetSocketNames.Num() <= 1)
		return;

	TArray<TWeakObjectPtr<AActor>> TargetActors;
	for (int i = 1; i < TargetSocketNames.Num(); ++i)
	{
		FVector Start = MeshComp->GetSocketLocation(TargetSocketNames[i-1]);
		FVector End = MeshComp->GetSocketLocation(TargetSocketNames[i]);

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQueries;
		ObjectTypeQueries.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> ActorsToIgnore;
		if (bIgnoreOwner)
		{
			ActorsToIgnore.Add(MeshComp->GetOwner());
		}

		TArray<FHitResult> HitResults;
		EDrawDebugTrace::Type DebugType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		if (UKismetSystemLibrary::SphereTraceMultiForObjects(MeshComp, 
			Start, End, TraceRadius, ObjectTypeQueries, false, ActorsToIgnore, DebugType, HitResults, true
			))
		{
			for (const FHitResult& HitResult : HitResults)
			{
				if (!IsValid(HitResult.GetActor()))
					continue;
				
				if (TargetActors.Contains(HitResult.GetActor()))
					continue;

				TargetActors.Add(HitResult.GetActor());
			}
		}
	}
	
	FGameplayEventData Payload;
	FGameplayAbilityTargetData_ActorArray* TargetDataActorArray = new FGameplayAbilityTargetData_ActorArray;
	TargetDataActorArray->SetActors(TargetActors);
	Payload.TargetData.Add(TargetDataActorArray);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), TargetEventTag, Payload);
}
