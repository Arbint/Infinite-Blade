// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/BAbilitySystemComponent.h"
#include "GAS/BAttributeSet.h"
#include "Widgets/OverheadStatWidget.h"

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

// Called when the game starts or when spawned
void ABCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverheadWidget();
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
	UE_LOG(LogTemp, Warning, TEXT("Dead"))
	PlayDeathAnimation();
}

void ABCharacter::Respawn()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawn"))
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
}

void ABCharacter::PlayDeathAnimation()
{
	PlayAnimMontage(DeathMontage);
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

