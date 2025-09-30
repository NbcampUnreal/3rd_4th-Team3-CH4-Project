#include "Gimmick/TTStealingtimeGimmick.h"
#include "Character/TTCharacterThief.h"
#include "Game/TTGameState.h"
#include "Kismet/GameplayStatics.h" 
#include "Components/SphereComponent.h"
#include "TimerManager.h"

ATTStealingtimeGimmick::ATTStealingtimeGimmick()
{
    // 틱은 필요 없으므로 false
    PrimaryActorTick.bCanEverTick = false;

    // 오버랩 종료 이벤트를 바인딩합니다. (ATTGimmickBase에는 BeginOverlap만 있으므로)
    // ATTGimmickBase::Trigger 컴포넌트가 유효한지 확인합니다.
    if (Trigger)
    {
        // Trigger의 크기 조정 (필요에 따라, 기본 80.f)
        Trigger->InitSphereRadius(200.f);

        Trigger->OnComponentEndOverlap.AddDynamic(this, &ATTStealingtimeGimmick::OnEndOverlap);
    }
}

// 오버랩 시작 (ATTGimmickBase::OnBeginOverlap 오버라이딩)
void ATTStealingtimeGimmick::OnBeginOverlap(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
    // 서버 권한과 이미 소모되었는지 확인
    if (!HasAuthority() || bConsumed)
    {
        return;
    }

    ATTCharacterThief* Thief = Cast<ATTCharacterThief>(Other);

    // 오버랩한 액터가 도둑이고, 현재 훔치기 진행 중인 도둑이 없을 때만 시작
    if (Thief && !OverlappingThief)
    {
        OverlappingThief = Thief;

        // 10초 후 StealingCompleted 함수를 실행하도록 타이머 설정
        GetWorldTimerManager().SetTimer(
            StealingTimerHandle,
            this,
            &ATTStealingtimeGimmick::StealingCompleted,
            StealDuration, // 기본 10초
            false // 반복 안 함
        );

    }
}

// 오버랩 종료
void ATTStealingtimeGimmick::OnEndOverlap(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
    if (!HasAuthority())
    {
        return;
    }

    ATTCharacterThief* Thief = Cast<ATTCharacterThief>(Other);

    // 오버랩을 끝낸 액터가 현재 훔치기를 진행 중이던 도둑일 경우
    if (Thief && Thief == OverlappingThief)
    {
        // 타이머 취소 및 도둑 정보 초기화
        GetWorldTimerManager().ClearTimer(StealingTimerHandle);
        OverlappingThief = nullptr;

    }
}

// 10초 오버랩 완료 시 호출
void ATTStealingtimeGimmick::StealingCompleted()
{
    if (!HasAuthority()) return;

    if (OverlappingThief)
    {

        bConsumed = true;

        // 이펙트 실행 (ExecuteEffect_Implementation 호출)
        ExecuteEffect(OverlappingThief);

        // 기믹 제거
        SetActorEnableCollision(false);
        Destroy();
    }

    OverlappingThief = nullptr;
}

// 이펙트 실행 (시간 절반 감소 로직)
void ATTStealingtimeGimmick::ExecuteEffect_Implementation(AActor* Activator)
{
    Super::ExecuteEffect_Implementation(Activator);

    // GameState를 가져와 남은 시간을 직접 감소시키는 로직
    if (ATTGameState* TTGameState = Cast<ATTGameState>(UGameplayStatics::GetGameState(GetWorld())))
    {
        if (HasAuthority())
        {
            const int32 TimeToSubtract = 60; // 1분 
            int32 CurrentTime = TTGameState->RemainingTime;

            // 1분을 감소시키고, 시간이 0 미만이 되는 것을 방지 (최소 1초는 남김)
            int32 NewRemainingTime = FMath::Max(1, CurrentTime - TimeToSubtract);

            // 서버에서 직접 RemainingTime 변수를 조작합니다.
            TTGameState->RemainingTime = NewRemainingTime;
        }
    }
}