// TTPCAnimInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TTPCAnimInstance.generated.h"

class ATTCharacterPolice;
class UCharacterMovementComponent;

UCLASS()
class TEAM03_API UTTPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<ATTCharacterPolice> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bShouldMove : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsFalling : 1;
};
