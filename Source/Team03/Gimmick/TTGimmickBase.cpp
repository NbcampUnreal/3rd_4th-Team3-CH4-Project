// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/TTGimmickBase.h"
#include "Components/SphereComponent.h"

// Sets default values
ATTGimmickBase::ATTGimmickBase()
{
    Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
    RootComponent = Trigger;
    Trigger->InitSphereRadius(80.f);
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetGenerateOverlapEvents(true);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    bReplicates = true;
}

// Called when the game starts or when spawned
void ATTGimmickBase::BeginPlay()
{
	Super::BeginPlay();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATTGimmickBase::OnBeginOverlap);
	
}

// Called every frame
void ATTGimmickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATTGimmickBase::OnBeginOverlap(UPrimitiveComponent*, AActor* Other, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    if (!Other) return;
    if (!HasAuthority()) return;            // 서버만 처리

    static bool bConsumed = false;          
    if (bConsumed) return;
    bConsumed = true;

    ExecuteEffect(Other);                    // 효과 적용 (속도 버프 등)

    SetActorEnableCollision(false);          // 더 이상 겹치지 않게
    Destroy();
}

void ATTGimmickBase::ExecuteEffect_Implementation(AActor* /*Activator*/)
{
    // 자식에서 구현
}