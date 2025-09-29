#include "Gimmick/TTeamWideSlowItem.h"
#include "Gimmick/TTStatModifierComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"              // GetWorld
#include "EngineUtils.h"               // FConstPawnIterator
#include "GameFramework/PlayerState.h" // (팀 시스템 연동시 참고 용)
#include "TimerManager.h"
#include "GameFramework/Actor.h"

ATTeamWideSlowItem::ATTeamWideSlowItem()
{
    // 디폴트 감속 값은 헤더의 SlowEffect 기본값 사용(필요시 BP에서 조절)
}

bool ATTeamWideSlowItem::IsPolice(const APawn* P) const
{
    if (!P) return false;
    return P->Tags.Contains(PoliceTag); // 간단: 태그로 판별
}

bool ATTeamWideSlowItem::IsThief(const APawn* P) const
{
    if (!P) return false;
    return P->Tags.Contains(ThiefTag);
}

void ATTeamWideSlowItem::ApplySlowToPawn(APawn* Target) const
{
    if (!Target) return;

    // 대상에게 스탯 컴포넌트를 찾습니다.
    UTTStatModifierComponent* Comp = Target->FindComponentByClass<UTTStatModifierComponent>();

    // 컴포넌트가 없으면 동적으로 생성 및 부착/초기화까지 안전하게 수행합니다.
    if (!Comp)
    {
        Comp = Cast<UTTStatModifierComponent>(
            Target->AddComponentByClass(
                UTTStatModifierComponent::StaticClass(),
                false,
                FTransform::Identity,
                false
            )
        );
    }

    // 감속은 Multiplier<1.0 사용. Additive를 +로 입력했다면 음수로 바꿔 적용.
    const float Additive = -FMath::Abs(SlowEffect.Additive);
    const float Multiplier = FMath::Clamp(SlowEffect.Multiplier, 0.1f, 1.0f);

    Comp->ApplyTemporarySpeedBoost(Additive, Multiplier, SlowEffect.Duration);
}

void ATTeamWideSlowItem::ExecuteEffect_Implementation(AActor* Activator)
{
    if (!HasAuthority()) return;

    APawn* InstigatorPawn = Cast<APawn>(Activator);
    if (!InstigatorPawn) return;

    const bool bActivatorIsPolice = IsPolice(InstigatorPawn);
    const bool bActivatorIsThief = IsThief(InstigatorPawn);
    if (!bActivatorIsPolice && !bActivatorIsThief) return;

    for (TActorIterator<APawn> It(GetWorld()); It; ++It)
    {
        APawn* P = *It;
        if (!IsValid(P) || P == InstigatorPawn) continue;

        const bool bTargetOppTeam =
            (bActivatorIsPolice && IsThief(P)) ||
            (bActivatorIsThief && IsPolice(P));

        if (bTargetOppTeam)
        {
            ApplySlowToPawn(P);
        }
    }
    // Destroy는 베이스에서 처리
}