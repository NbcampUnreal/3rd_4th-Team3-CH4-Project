// TTCharacterPolice.h

#pragma once

#include "CoreMinimal.h"
#include "Character/TTCharacterBase.h"
#include "TTCharacterPolice.generated.h"

class UTTPoliceInput;

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
};