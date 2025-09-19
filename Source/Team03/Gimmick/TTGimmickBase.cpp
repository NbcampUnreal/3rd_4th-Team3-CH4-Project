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

    // 서버만 효과 실행 (클라는 무시)
    if (!HasAuthority()) return;

    ExecuteEffect(Other);
}

void ATTGimmickBase::ExecuteEffect_Implementation(AActor* /*Activator*/)
{
    // 자식에서 구현
}