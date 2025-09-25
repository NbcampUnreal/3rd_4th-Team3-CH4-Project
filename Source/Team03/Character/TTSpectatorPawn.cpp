// TTSpectatorPawn.cpp

#include "Character/TTSpectatorPawn.h"
#include "Character/TTPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"

ATTSpectatorPawn::ATTSpectatorPawn()
{
	// SpectatorPawn 기본 MoveementComponent 제거
	if(UFloatingPawnMovement* FM = Cast<UFloatingPawnMovement>(MovementComponent))
	{
		FM->DestroyComponent();
		MovementComponent = nullptr;
	}
}

void ATTSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
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
}

void ATTSpectatorPawn::OnSpectatorNextViewTarget(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("OnSpectatorNextViewTarget!"));
}
