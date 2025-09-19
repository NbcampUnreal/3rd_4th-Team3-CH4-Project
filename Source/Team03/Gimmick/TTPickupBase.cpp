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
        Comp = NewObject<UTTStatModifierComponent>(Pawn);
        Comp->RegisterComponent();
        Pawn->AddInstanceComponent(Comp);
    }

    Comp->ApplyTemporarySpeedBoost(SpeedEffect.Additive, SpeedEffect.Multiplier, SpeedEffect.Duration);
}
