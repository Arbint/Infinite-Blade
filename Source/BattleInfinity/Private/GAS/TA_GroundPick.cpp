// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TA_GroundPick.h"
#include "TA_GroundPick.h"

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
