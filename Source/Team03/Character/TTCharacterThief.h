// TTCharacterThief.h

#pragma once

#include "CoreMinimal.h"
#include "Character/TTCharacterBase.h"
#include "TTCharacterThief.generated.h"

class UTTCharacterThiefData;
class UTTBaseStatComponent;

UCLASS()
class TEAM03_API ATTCharacterThief : public ATTCharacterBase
{
	GENERATED_BODY()
	
public:
	ATTCharacterThief();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser)
		override;

	// bIsDead Getter 함수 추가
	uint8 IsDead() const { return bIsDead; }

private:
	UFUNCTION()
	void OnRep_SelectMesh();

	UFUNCTION()
	void OnRep_IsDead();

	void ActivateRagdoll();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_IsDead)
	uint8 bIsDead;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UTTBaseStatComponent* BaseStatComp; 

private:
	UPROPERTY(EditAnywhere, Category="DataAsset")
	TObjectPtr<UTTCharacterThiefData> ThiefMeshData;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SelectMesh)
	int32 Index;
};
