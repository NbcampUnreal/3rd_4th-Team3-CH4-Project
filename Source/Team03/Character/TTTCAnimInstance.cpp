// TTTCAnimInstance.cpp

#include "Character/TTTCAnimInstance.h"
#include "Character/TTCharacterThief.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTTTCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ATTCharacterThief>(GetOwningActor());
	if(IsValid(OwnerCharacter) == true)
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UTTTCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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