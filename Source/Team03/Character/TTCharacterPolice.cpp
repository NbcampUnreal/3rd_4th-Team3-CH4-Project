// TTCharacterPolice.cpp

#include "Character/TTCharacterPolice.h"
#include "GameFramework/CharacterMovementComponent.h"

ATTCharacterPolice::ATTCharacterPolice()
{
    // 경찰 캐릭터 무브먼트 관련 수치 조정
    BaseWalkSpeed = 500;
    BaseSprintSpeed = 800;

    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

float ATTCharacterPolice::GetDefaultWalkSpeed() const
{
    return BaseWalkSpeed;
}

float ATTCharacterPolice::GetSprintWalkSpeed() const
{
    return BaseSprintSpeed;
}
