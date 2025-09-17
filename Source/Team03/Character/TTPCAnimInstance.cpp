// TTPCAnimInstance.cpp

#include "Character/TTPCAnimInstance.h"
#include "Character/TTCharacterPolice.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTTPCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ATTCharacterPolice>(GetOwningActor());
	if(IsValid(OwnerCharacter) == true)
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UTTPCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(IsValid(OwnerCharacter) == false || IsValid(OwnerCharacterMovementComponent) == false)
	{
		return;
	}

	Velocity = OwnerCharacterMovementComponent->Velocity;
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	bShouldMove = ((OwnerCharacterMovementComponent->GetCurrentAcceleration().IsNearlyZero()) == false) && (3.f < GroundSpeed);
	bIsFalling = OwnerCharacterMovementComponent->IsFalling();
}
