// TTBaseStatComponent.cpp

#include "Component/TTBaseStatComponent.h"
#include "Net/UnrealNetwork.h"

UTTBaseStatComponent::UTTBaseStatComponent():
    CurrentHP(100.f),
    MaxHP(100.f)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

float UTTBaseStatComponent::ApplyDamage(float InDamage)
{
    const float PreviousHP = CurrentHP;
    const float ActualDamage = FMath::Clamp<float>(InDamage, 0.f, PreviousHP);

	SetCurrentHP(PreviousHP - ActualDamage);

	UE_LOG(LogTemp, Error, TEXT("%f"), GetCurrentHP());

    return ActualDamage;
}

void UTTBaseStatComponent::SetCurrentHP(float InCurrentHP)
{
    CurrentHP = InCurrentHP;
    if(CurrentHP <= KINDA_SMALL_NUMBER)
    {
        CurrentHP = 0.f;
		OnOutOfCurrentHP.Broadcast();
    }
	OnCurrentHPChanged.Broadcast(CurrentHP);
}

void UTTBaseStatComponent::SetMaxHP(float InMaxHP)
{
    MaxHP = InMaxHP;

    if(MaxHP < KINDA_SMALL_NUMBER)
    {
        MaxHP = 0.f;
    }

	OnMaxHPChanged.Broadcast(MaxHP);
}

void UTTBaseStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UTTBaseStatComponent, CurrentHP);
    DOREPLIFETIME(UTTBaseStatComponent, MaxHP);
}

void UTTBaseStatComponent::OnRep_CurrentHP()
{
	OnCurrentHPChanged.Broadcast(CurrentHP);
}

void UTTBaseStatComponent::OnRep_MaxHP()
{
	OnMaxHPChanged.Broadcast(MaxHP);
}
