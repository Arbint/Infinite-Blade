// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/BAbilitySystemComponent.h"
#include "GAS/BAttributeSet.h"
#include "Widgets/OverheadStatWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABCharacter::ABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BAbilitySystemComponent = CreateDefaultSubobject<UBAbilitySystemComponent>("BAbilitySystemComponent");
	BAttributeSet = CreateDefaultSubobject<UBAttributeSet>("BAttributeSet");

	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Overhead Widget Component");
	OverheadWidgetComponent->SetupAttachment(GetRootComponent());
	BindAbilitySystemDelegates();
}

void ABCharacter::ServerSideInit()
{
	if (BAbilitySystemComponent)
	{
		BAbilitySystemComponent->InitAbilityActorInfo(this, this);
		BAbilitySystemComponent->ApplyInitialEffects();
		BAbilitySystemComponent->GrantInitialAbilities();
	}
}

void ABCharacter::ClientSideInit()
{
	if (BAbilitySystemComponent)
	{
		BAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

void ABCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABCharacter, TeamId);
}

// Called when the game starts or when spawned
void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverheadWidget();
	CachedMeshRelativeTransform = GetMesh()->GetRelativeTransform();

	if (HasAuthority() && GetController() && !GetController()->IsPlayerController())
	{
		ServerSideInit();
	}
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

void ABCharacter::BindAbilitySystemDelegates()
{
	if (BAbilitySystemComponent)
	{
		BAbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("stat.dead"))
			.AddUObject(this, &ABCharacter::DeadTagUpdated);
	}
}

void ABCharacter::DeadTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount != 0)
	{
		StartDeathSequence();
	}
	else
	{
		Respawn();
	}
}

void ABCharacter::StartDeathSequence()
{
	OnDead();
	UE_LOG(LogTemp, Warning, TEXT("Dead"))
	PlayDeathAnimation();
	OverheadWidgetComponent->SetHiddenInGame(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void ABCharacter::Respawn()
{
	SetEnableRagdoll(false);
	OnRespawn();
	UE_LOG(LogTemp, Warning, TEXT("Respawn"))
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	ConfigureOverheadWidget();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	if (HasAuthority() && GetController() && GetController()->StartSpot.IsValid())
	{
		SetActorTransform(GetController()->StartSpot->GetActorTransform());
	}
}

void ABCharacter::OnDead()
{
	//Override in child class
}

void ABCharacter::OnRespawn()
{
	//Override in child class
}

void ABCharacter::PlayDeathAnimation()
{
	float DeathMontageDuration = PlayAnimMontage(DeathMontage);
	FTimerHandle DeathMontagePlayTimerHandle;
	GetWorldTimerManager().SetTimer(DeathMontagePlayTimerHandle, this, &ABCharacter::DeathMontageFinished, DeathMontageDuration - 0.8f);
}

void ABCharacter::SetEnableRagdoll(bool bEnableRagdoll)
{
	// the mesh needs to be detached when in ragdoll
	// the mesh should start simulating physics when in ragdoll
	if (bEnableRagdoll)
	{
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(CachedMeshRelativeTransform);
	}
}

void ABCharacter::DeathMontageFinished()
{
	SetEnableRagdoll(true);
}

void ABCharacter::ConfigureOverheadWidget()
{
	if (!OverheadWidgetComponent)
		return;

	if (GetController() && GetController()->IsLocalPlayerController())
	{
		OverheadWidgetComponent->SetHiddenInGame(true);
		return;
	}

	UOverheadStatWidget* OverheadStatWidget = Cast<UOverheadStatWidget>(OverheadWidgetComponent->GetUserWidgetObject());
	if (OverheadStatWidget)
	{
		OverheadStatWidget->InitializeWithAbilitySystemComponent(GetAbilitySystemComponent());
		OverheadWidgetComponent->SetHiddenInGame(false);
	}
}

