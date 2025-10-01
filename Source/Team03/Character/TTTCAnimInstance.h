// TTTCAnimInstance.h

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TTTCAnimInstance.generated.h"

class ATTCharacterThief;
class UCharacterMovementComponent;

UCLASS()
class TEAM03_API UTTTCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	TObjectPtr<ATTCharacterThief> OwnerCharacter;

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
