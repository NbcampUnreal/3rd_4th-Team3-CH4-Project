// TTCharacterBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TTCharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTTBaseCharacterInput;
class UInputMappingContext;

UCLASS()
class TEAM03_API ATTCharacterBase : public ACharacter
{
	GENERATED_BODY()

// 캐릭터 관련 컴포넌트 관리 및 수치 조정
#pragma region ACharacter

public:
	ATTCharacterBase();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual float GetDefaultWalkSpeed() const;
	
	virtual float GetSprintWalkSpeed() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* SprintArmComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComp;

#pragma endregion

// 입력 시스템 관련 변수 및 함수
#pragma region Input

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="DataAsset")
	TObjectPtr<UTTBaseCharacterInput> BaseCharacterInputData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="IMC")
	TObjectPtr<UInputMappingContext> BaseCharcterIMC;

private:
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float BaseSprintSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;

#pragma endregion
};
