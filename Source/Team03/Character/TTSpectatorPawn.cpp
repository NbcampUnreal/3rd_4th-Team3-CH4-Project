// TTSpectatorPawn.cpp

#include "Character/TTSpectatorPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ATTSpectatorPawn::ATTSpectatorPawn()
{

}

void ATTSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();

	/*if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if(IsValid(SubSystem))
			{
				UE_LOG(LogTemp, Error, TEXT("SubSystem Call"));
				SubSystem->ClearAllMappings();
				SubSystem->AddMappingContext(SpectatorIMC, 0);
			}
		}
	}*/
}

void ATTSpectatorPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if(IsValid(EnhancedInputComp))
	{
		EnhancedInputComp->BindAction(SpectatorPreViewTarget,
									  ETriggerEvent::Triggered,
									  this,
									  &ThisClass::OnSpectatorPreViewTarget);
		EnhancedInputComp->BindAction(SpectatorNextViewTarget,
									  ETriggerEvent::Triggered,
									  this,
									  &ThisClass::OnSpectatorNextViewTarget);
	}
}

void ATTSpectatorPawn::OnSpectatorPreViewTarget(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("OnSpectatorPreViewTarget!"));
}

void ATTSpectatorPawn::OnSpectatorNextViewTarget(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("OnSpectatorNextViewTarget!"));
}
