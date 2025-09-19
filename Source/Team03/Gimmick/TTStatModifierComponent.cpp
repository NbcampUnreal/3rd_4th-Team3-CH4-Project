// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TTStatModifierComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTTStatModifierComponent::UTTStatModifierComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTTStatModifierComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const ACharacter* C = Cast<ACharacter>(GetOwner()))
	{
		if (const auto* Move = C->GetCharacterMovement())
		{
			BaseWalkSpeed = FMath::Max(50.f, Move->MaxWalkSpeed);
		}
	}
	
}


// Called every frame
void UTTStatModifierComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTTStatModifierComponent::ApplyTemporarySpeedBoost(float Additive, float Multiplier, float DurationSeconds)
{
	if (ACharacter* C = Cast<ACharacter>(GetOwner()))
	{
		if (auto* Move = C->GetCharacterMovement())
		{
			PreviousSpeed = Move->MaxWalkSpeed; // 현재 속도 스냅샷
			const float Final = FMath::Clamp((PreviousSpeed + Additive) * Multiplier, 100.f, 1200.f);
			Move->MaxWalkSpeed = Final;
		}
	}

	if (UWorld* W = GetWorld())
	{
		W->GetTimerManager().ClearTimer(RestoreHandle);
		if (DurationSeconds > 0.f)
		{
			W->GetTimerManager().SetTimer(
				RestoreHandle, this, &UTTStatModifierComponent::RestoreSpeed, DurationSeconds, false
			);
		}
	}
}


void UTTStatModifierComponent::RestoreSpeed()
{
	if (ACharacter* C = Cast<ACharacter>(GetOwner()))
		if (auto* Move = C->GetCharacterMovement())
		{
			Move->MaxWalkSpeed = PreviousSpeed; // ✅ 추가: 적용 전 속도로 원복
		}
}