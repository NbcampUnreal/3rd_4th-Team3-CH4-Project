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

protected:

	UPROPERTY()
	UCharacterMovementComponent* Move;

	UPROPERTY()
	float BaseWalkSpeed = 600.f;

	UPROPERTY()
	FTimerHandle SpeedTimer;


	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_ApplySpeedBoost(float NewSpeed);
	void NetMulticast_ApplySpeedBoost_Implementation(float NewSpeed);

	void RestoreSpeed();

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	

	void ApplyTemporarySpeedBoost(float Additive, float Multiplier, float Duration);
	
private:

};
