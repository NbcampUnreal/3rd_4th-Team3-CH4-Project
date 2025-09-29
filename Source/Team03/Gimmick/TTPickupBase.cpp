#include "Gimmick/TTPickupBase.h"
#include "Gimmick/TTStatModifierComponent.h"
#include "GameFramework/Pawn.h"

void ATTPickupBase::ExecuteEffect_Implementation(AActor* Activator)
{
    if (!HasAuthority()) return;

    APawn* Pawn = Cast<APawn>(Activator);
    if (!Pawn) return;

    UTTStatModifierComponent* Comp = Pawn->FindComponentByClass<UTTStatModifierComponent>();

   
    if (!Comp)
    {
        Comp = Cast<UTTStatModifierComponent>(
            Pawn->AddComponentByClass(
                UTTStatModifierComponent::StaticClass(),
                false,
                FTransform::Identity,
                false
            )
        );
    }

    // 이 시점에서 Comp는 무조건 유효함
    if (Comp)
    {
        Comp->ApplyTemporarySpeedBoost(SpeedEffect.Additive, SpeedEffect.Multiplier, SpeedEffect.Duration);
    }
}