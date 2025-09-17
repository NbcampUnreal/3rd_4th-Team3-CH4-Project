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
	/*
	데미지 처리용 함수 개인 메모 추후 삭제
	DamageAmount : 가해지는 데미지의 양
	DamageEvent : 데미지의 타입과 추가 데이터를 담은 구조체
	EventInstigator : 데미지를 가한 주체를 조종하는 컨트롤러
	DamageCause : 실제로 데미지를 발생시킨 액터
	*/
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void CheckMeleeAttackHit();

private:
	void DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);

protected:
	bool bCanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> MeleeAttackMontage;

	float MeleeAttackMontagePlayTime;

#pragma endregion
};