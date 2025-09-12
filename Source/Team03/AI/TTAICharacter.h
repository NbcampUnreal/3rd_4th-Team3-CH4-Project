#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TTAICharacter.generated.h"

class UAnimMontage;
class UTTAIDataAsset;

UCLASS()
class TEAM03_API ATTAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATTAICharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_IsDead)
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathMontage;

	UFUNCTION()
	void OnRep_IsDead();

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	TObjectPtr<UTTAIDataAsset> AIMeshDataAsset;
};
