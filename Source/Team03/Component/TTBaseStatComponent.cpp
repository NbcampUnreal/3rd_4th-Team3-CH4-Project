// TTBaseStatComponent.cpp

#include "Component/TTBaseStatComponent.h"
#include "Net/UnrealNetwork.h"

UTTBaseStatComponent::UTTBaseStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UTTBaseStatComponent::BeginPlay()
{
    Super::BeginPlay();

    // 서버에서만 MaxHP 설정
    if(GetOwnerRole() == ROLE_Authority)
    {
        HP = MaxHP;
    }
}

void UTTBaseStatComponent::OnRep_HP()
{
    // 클라이언트에서 HP가 갱신될 때 호출
}

void UTTBaseStatComponent::ApplyDamage(float Damage)
{
    if(GetOwnerRole() == ROLE_Authority)
    {
        HP = FMath::Clamp(HP - Damage, 0.f, MaxHP);
    }
}

void UTTBaseStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTTBaseStatComponent, HP);
}