// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TA_GroundPick.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/OverlapResult.h"
#include "GenericTeamAgentInterface.h"
#include "TA_GroundPick.h"
#include "AbilitySystemBlueprintLibrary.h"

ATA_GroundPick::ATA_GroundPick()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATA_GroundPick::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (PrimaryPC && PrimaryPC->IsLocalPlayerController())
	{
		SetActorLocation(GetTargetingAimLoc());
	}
}

void ATA_GroundPick::ConfirmTargetingAndContinue()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(TargetAreaRadius);

	GetWorld()->OverlapMultiByObjectType(OverlapResults, GetActorLocation(),
		FQuat::Identity, ObjectQueryParams, CollisionShape 	
		);
	
	IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(
		OwningAbility->GetAvatarActorFromActorInfo());

	TSet<AActor*> TargetActors;
	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		if (!OverlapResult.GetActor() || !OwnerTeamInterface)
			continue;

		if (OwnerTeamInterface->GetTeamAttitudeTowards(*OverlapResult.GetActor()) == ETeamAttitude::Friendly && !bDetectFriendly)
			continue;

		if (OwnerTeamInterface->GetTeamAttitudeTowards(*OverlapResult.GetActor()) == ETeamAttitude::Hostile && !bDetectEnemy)
			continue;

		TargetActors.Add(OverlapResult.GetActor());
	}
	TargetDataReadyDelegate.Broadcast(UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActorArray(TargetActors.Array(), false));
}

FVector ATA_GroundPick::GetTargetingAimLoc() const
{
	if(!PrimaryPC)
		return GetActorLocation();
	
	FVector ViewLoc;
	FRotator ViewRot;
	PrimaryPC->GetPlayerViewPoint(ViewLoc, ViewRot);
	FVector TraceEnd = ViewLoc + ViewRot.Vector() * TargetMaxDistance;

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, ViewLoc, TraceEnd, ECC_Visibility);
	if (!HitResult.bBlockingHit)
	{
		GetWorld()->LineTraceSingleByChannel(HitResult, 
			TraceEnd, 
			TraceEnd + FVector::DownVector * TNumericLimits<float>::Max(), ECC_Visibility);
	}

	if (HitResult.bBlockingHit)
		return HitResult.ImpactPoint;

	return GetActorLocation();
}
