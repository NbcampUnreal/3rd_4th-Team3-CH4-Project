// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTStatModifierComponent.generated.h"


class UCharacterMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM03_API UTTStatModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTTStatModifierComponent();


	float GetCurrentAdditive() const { return CurrentAdditive; }
	float GetCurrentMultiplier() const { return CurrentMultiplier; }

protected:

	UPROPERTY()
	UCharacterMovementComponent* Move;

	UPROPERTY()
	float BaseWalkSpeed = 600.f;

	UPROPERTY()
	FTimerHandle SpeedTimer;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentModifiers)
		float CurrentAdditive = 0.f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentModifiers)
		float CurrentMultiplier = 1.0f;


	void RestoreSpeed();

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	

	void ApplyTemporarySpeedBoost(float Additive, float Multiplier, float Duration);
	
private:
	UFUNCTION()
	void OnRep_CurrentModifiers();
	void UpdateMovementSpeed();
};
