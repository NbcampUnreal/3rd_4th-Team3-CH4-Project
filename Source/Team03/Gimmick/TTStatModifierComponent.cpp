// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TTStatModifierComponent.h"
#include "Character/TTCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h" 

// Sets default values for this component's properties
UTTStatModifierComponent::UTTStatModifierComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UTTStatModifierComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Move = Character->GetCharacterMovement();
		if (Move)
		{
			BaseWalkSpeed = Move->MaxWalkSpeed;
		}
	}
}



void UTTStatModifierComponent::ApplyTemporarySpeedBoost(float Additive, float Multiplier, float Duration)
{
	if (!GetOwner()->HasAuthority()) return;

	CurrentAdditive = Additive;
	CurrentMultiplier = Multiplier;

	// RepNotify를 통해 클라이언트에도 즉시 동기화될 것입니다.

	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(SpeedTimer);
		W->GetTimerManager().SetTimer(
			SpeedTimer,
			this,
			&UTTStatModifierComponent::RestoreSpeed,
			Duration,
			false
		);
	}
}

void UTTStatModifierComponent::RestoreSpeed()
{
	if (!GetOwner()->HasAuthority()) return;

	CurrentAdditive = 0.f;
	CurrentMultiplier = 1.0f;

	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(SpeedTimer);
	}

}
void UTTStatModifierComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTTStatModifierComponent, CurrentAdditive);
	DOREPLIFETIME(UTTStatModifierComponent, CurrentMultiplier);
}
