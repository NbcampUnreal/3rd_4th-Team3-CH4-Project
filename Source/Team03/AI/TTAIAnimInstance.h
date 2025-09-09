#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TTAIAnimInstance.generated.h"

UCLASS()
class TEAM03_API UTTAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float Speed;

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
