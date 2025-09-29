// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TTGimmickBase.generated.h"

class USphereComponent;

UCLASS(Abstract)
class TEAM03_API ATTGimmickBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATTGimmickBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere) USphereComponent* Trigger;

	UPROPERTY()
	bool bConsumed = false;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Sweep);


	UFUNCTION(BlueprintNativeEvent) void ExecuteEffect(AActor* Activator);
	virtual void ExecuteEffect_Implementation(AActor* Activator);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
