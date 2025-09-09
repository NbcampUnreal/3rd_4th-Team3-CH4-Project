#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TTAICharacter.generated.h"

UCLASS()
class TEAM03_API ATTAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATTAICharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsDead;

protected:
	virtual void BeginPlay() override;



};
