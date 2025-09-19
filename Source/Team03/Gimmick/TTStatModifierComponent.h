// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTStatModifierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM03_API UTTStatModifierComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTTStatModifierComponent();

	UPROPERTY() float PreviousSpeed = 600.f;

	UFUNCTION(BlueprintCallable, Category = "Stat|Speed")
	void ApplyTemporarySpeedBoost(float Additive, float Multiplier, float DurationSeconds);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY() float BaseWalkSpeed = 600.f;   // 원래 속도(시작 시 캐시)
	FTimerHandle RestoreHandle;

	void RestoreSpeed();
};
