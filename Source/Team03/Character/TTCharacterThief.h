// TTCharacterThief.h

#pragma once

#include "CoreMinimal.h"
#include "Character/TTCharacterBase.h"
#include "TTCharacterThief.generated.h"

class UTTCharacterThiefData;
class UTTBaseStatComponent;
class ATTSpectatorPawn;

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

private:
	UFUNCTION()
	void OnRep_SelectMesh();

public:

	UPROPERTY(EditDefaultsOnly, Category="Spectator")
	TSubclassOf<ATTSpectatorPawn> SpectatorPawnClass;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UTTBaseStatComponent* BaseStatComp; 

private:
	UPROPERTY(EditAnywhere, Category="DataAsset")
	TObjectPtr<UTTCharacterThiefData> ThiefMeshData;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SelectMesh)
	int32 Index;
};
