#include "AI/TTAIAnimInstance.h"

void UTTAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Speed = 0.f;
}

void UTTAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn) return;

	Speed = OwnerPawn->GetVelocity().Size();
}
