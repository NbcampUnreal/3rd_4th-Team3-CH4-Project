// TTCharacterPolice.h

#pragma once

#include "CoreMinimal.h"
#include "Character/TTCharacterBase.h"
#include "TTCharacterPolice.generated.h"

class UTTPoliceInput;
class UAnimMontage;

UCLASS()
class TEAM03_API ATTCharacterPolice : public ATTCharacterBase
{
	GENERATED_BODY()
	
#pragma region ACharacter

public:
	ATTCharacterPolice();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual float GetDefaultWalkSpeed() const override;

	virtual float GetSprintWalkSpeed() const override;

#pragma endregion

#pragma region Input

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DataAsset")
	TObjectPtr<UTTPoliceInput> PoliceCharacterInputData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="IMC")
	TObjectPtr<UInputMappingContext> PoliceCharacterIMC;

private:
	UFUNCTION()
	void MeleeAttack(const FInputActionValue& Value);

#pragma endregion

#pragma region Attack
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void CheckMeleeAttackHit();

private:
	void DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);

	void PlayMeleeAttackMontage();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> MeleeAttackMontage;

	float MeleeAttackMontagePlayTime;

#pragma endregion

#pragma region Replication

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_CanAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCMeleeAttack(float InStartMeleeAttackTime);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCMeleeAttack();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCPerformMeleeHit(ACharacter* InDamagedCharacters, float InCheckTime);

protected:
	UPROPERTY(ReplicatedUsing=OnRep_CanAttack)
	uint8 bCanAttack : 1;

	// 지난 근접 공격 시작 시간
	float LastStartMeleeAttackTime;

	// 근접 공격 시간 차이
	float MeleeAttackTimeDifference;

	// 근접 공격 허용 최소 시간 간격
	float MinAllowedTimeForMeleeAttack;

#pragma endregion
};