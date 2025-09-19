#pragma once
#include "CoreMinimal.h"
#include "Gimmick/TTGimmickBase.h"
#include "TTPickupBase.generated.h"

USTRUCT(BlueprintType)
struct FTimedSpeedEffect
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere) float Additive = 0.f;
    UPROPERTY(EditAnywhere) float Multiplier = 1.f;
    UPROPERTY(EditAnywhere) float Duration = 5.f;
};

UCLASS(Abstract)
class TEAM03_API ATTPickupBase : public ATTGimmickBase
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, Category = "Pickup") FTimedSpeedEffect SpeedEffect;

    virtual void ExecuteEffect_Implementation(AActor* Activator) override;
};
