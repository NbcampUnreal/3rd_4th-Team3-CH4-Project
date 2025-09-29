// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TTStatModifierComponent.h"
#include "Character/TTCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	if (!Move) return;

	const float Final = FMath::Clamp((BaseWalkSpeed + Additive) * Multiplier, 100.f, 1200.f);

	Move->MaxWalkSpeed = Final;
	NetMulticast_ApplySpeedBoost(Final);

	
	if (UWorld* W = GetWorld())
	{
		// 기존 타이머가 있다면 취소하고 새로 설정
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




void UTTStatModifierComponent::NetMulticast_ApplySpeedBoost_Implementation(float NewSpeed)
{
	if (!Move) return;
	Move->MaxWalkSpeed = NewSpeed;
}

void UTTStatModifierComponent::RestoreSpeed()
{
	if (!GetOwner()->HasAuthority()) return;

	if (!Move) return;

	
	Move->MaxWalkSpeed = BaseWalkSpeed;
	NetMulticast_ApplySpeedBoost(BaseWalkSpeed); // NetMulticast_ApplySpeedBoost 재사용

	
	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(SpeedTimer);
	}
}

