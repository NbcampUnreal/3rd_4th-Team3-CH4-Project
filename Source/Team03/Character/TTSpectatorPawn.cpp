// TTSpectatorPawn.cpp

#include "Character/TTSpectatorPawn.h"
#include "Character/TTPlayerController.h"
#include "Character/TTCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

ATTSpectatorPawn::ATTSpectatorPawn()
{
	// SpectatorPawn 기본 MoveementComponent 제거
	if(UFloatingPawnMovement* FM = Cast<UFloatingPawnMovement>(MovementComponent))
	{
		FM->DestroyComponent();
		MovementComponent = nullptr;
	}

	CurrentViewTargetIndex = 0;
}

void ATTSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어 Pawn 목록 초기화
	RefreshViewTargetList();
}

void ATTSpectatorPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(const auto* PC = Cast<ATTPlayerController>(GetController()))
	{
		if(auto* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(SpectatorIMC, 0);
		}
	}

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(IsValid(EnhancedInputComp))
	{
		EnhancedInputComp->BindAction(SpectatorPreViewTarget,
									  ETriggerEvent::Started,
									  this,
									  &ThisClass::OnSpectatorPreViewTarget);
		EnhancedInputComp->BindAction(SpectatorNextViewTarget,
									  ETriggerEvent::Started,
									  this,
									  &ThisClass::OnSpectatorNextViewTarget);
	}
}

void ATTSpectatorPawn::OnSpectatorPreViewTarget(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("OnSpectatorPreViewTarget!"));

	if(ViewTargets.Num() == 0) return;

	CurrentViewTargetIndex = (CurrentViewTargetIndex - 1 + ViewTargets.Num()) % ViewTargets.Num();
	SwitchToViewTarget(ViewTargets[CurrentViewTargetIndex]);
}

void ATTSpectatorPawn::OnSpectatorNextViewTarget(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("OnSpectatorNextViewTarget!"));

	if(ViewTargets.Num() == 0) return;

	CurrentViewTargetIndex = (CurrentViewTargetIndex + 1) % ViewTargets.Num();
	SwitchToViewTarget(ViewTargets[CurrentViewTargetIndex]);
}

void ATTSpectatorPawn::RefreshViewTargetList()
{
	ViewTargets.Empty();

	UWorld* World = GetWorld();
	if(!World) return;

	// 현재 존재하는 모든 Pawn 검색
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(World, ATTCharacterBase::StaticClass(), FoundCharacters);

	for(AActor* Actor : FoundCharacters)
	{
		ATTCharacterBase* Character = Cast<ATTCharacterBase>(Actor);
		if(Character && !Character->IsDead())
		{
			ViewTargets.Add(Character);
		}
	}
}

void ATTSpectatorPawn::SwitchToViewTarget(APawn* TargetPawn)
{
	if(!TargetPawn) return;

	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetViewTargetWithBlend(
			TargetPawn,
			0.0f
		);
	}
}