// TTBaseStatComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTBaseStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM03_API UTTBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTTBaseStatComponent();

	void ApplyDamage(float Damage);

	float GetHP() const { return HP; }

	float GetMaxHP() const { return MaxHP; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

protected:
	UPROPERTY(ReplicatedUsing=OnRep_HP, VisibleAnywhere, Category="Stat")
	float HP;

	UPROPERTY(EditDefaultsOnly, Category="Stat")
	float MaxHP = 100.f;

	UFUNCTION()
	void OnRep_HP();
};
