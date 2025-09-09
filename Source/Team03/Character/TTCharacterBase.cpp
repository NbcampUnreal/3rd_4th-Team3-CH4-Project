// TTCharacterBase.cpp

#include "Character/TTCharacterBase.h"

ATTCharacterBase::ATTCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}