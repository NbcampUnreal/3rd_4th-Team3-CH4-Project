// TTBaseStatComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTBaseStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHPChangedDelegate, float /* InCurrentHP */);
DECLARE_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHPChangedDelegate, float /* InMaxHP */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM03_API UTTBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTTBaseStatComponent();

	float ApplyDamage(float InDamage);

	float GetCurrentHP() const { return CurrentHP; }

	void SetCurrentHP(float InCurrentHP);

	float GetMaxHP() const { return MaxHP; }

	void SetMaxHP(float InMaxHP);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentHP)
	float CurrentHP;

	UPROPERTY(ReplicatedUsing=OnRep_MaxHP)
	float MaxHP;

	FOnCurrentHPChangedDelegate OnCurrentHPChanged;

	FOnOutOfCurrentHPDelegate OnOutOfCurrentHP;

	FOnMaxHPChangedDelegate OnMaxHPChanged;

protected:
	UFUNCTION()
	void OnRep_CurrentHP();

	UFUNCTION()
	void OnRep_MaxHP();
};
