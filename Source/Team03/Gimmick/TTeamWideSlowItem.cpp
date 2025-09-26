#include "Gimmick/TTeamWideSlowItem.h"
#include "Gimmick/TTStatModifierComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"              // GetWorld
#include "EngineUtils.h"               // FConstPawnIterator
#include "GameFramework/PlayerState.h" // (팀 시스템 연동시 참고 용)
#include "TimerManager.h"

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

    // 대상에게 스탯 컴포넌트를 찾아서(없으면 동적 생성) 감속 적용
    UTTStatModifierComponent* Comp = Target->FindComponentByClass<UTTStatModifierComponent>();
    if (!Comp)
    {
        Comp = NewObject<UTTStatModifierComponent>(Target);
        Comp->RegisterComponent();
        // 런타임 동적 추가 시에는 AddOwnedComponent가 더 적절함(소유/GC 관리)
        Target->AddOwnedComponent(Comp);
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