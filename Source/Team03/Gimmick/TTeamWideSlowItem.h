#pragma once

#include "CoreMinimal.h"
#include "Gimmick/TTGimmickBase.h"
#include "TTeamWideSlowItem.generated.h"

class APawn;

/** (데이터) 속도 디버프 수치 */
USTRUCT(BlueprintType)
struct FTimedSlowEffect
{
    GENERATED_BODY()

    // 0.5배 = 50% 감속 (항상 0~1 사이로 제한)
    UPROPERTY(EditAnywhere, Category = "Effect", meta = (ClampMin = "0.1", ClampMax = "1.0"))
    float Multiplier = 0.50f;

    // 지속시간(초)
    UPROPERTY(EditAnywhere, Category = "Effect", meta = (ClampMin = "0.0"))
    float Duration = 5.0f;

    // 절댓값 감속(원하면 사용). +로 적으면 코드에서 자동으로 -로 바꿔줌
    UPROPERTY(EditAnywhere, Category = "Effect")
    float Additive = 0.f;
};

UCLASS()
class TEAM03_API ATTeamWideSlowItem : public ATTGimmickBase
{
    GENERATED_BODY()

public:
    ATTeamWideSlowItem();

protected:
    /** 팀 판별용 태그(간단/유연): Pawn->Tags 에 "Team.Police", "Team.Thief" 붙여두면 됨 */
    UPROPERTY(EditAnywhere, Category = "Team")
    FName PoliceTag = TEXT("Team.Police");

    UPROPERTY(EditAnywhere, Category = "Team")
    FName ThiefTag = TEXT("Team.Thief");

    /** 적용할 디버프 데이터 */
    UPROPERTY(EditAnywhere, Category = "Effect")
    FTimedSlowEffect SlowEffect;

    /** 실제 효과: 오버랩한 Pawn의 '반대 팀' 전원에게 감속 적용 */
    virtual void ExecuteEffect_Implementation(AActor* Activator) override;

    /** 팀 판별 도우미(태그 기반) */
    bool IsPolice(const APawn* P) const;
    bool IsThief(const APawn* P) const;

    /** 한 Pawn에 디버프 1회 적용 */
    void ApplySlowToPawn(APawn* Target) const;
};