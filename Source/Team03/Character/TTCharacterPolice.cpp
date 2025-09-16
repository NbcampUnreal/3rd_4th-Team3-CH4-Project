// TTCharacterPolice.cpp

#include "Character/TTCharacterPolice.h"
#include "Input/TTPoliceInput.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

ATTCharacterPolice::ATTCharacterPolice()
{
    // 경찰 캐릭터 무브먼트 관련 수치 조정
    BaseWalkSpeed = 500;
    BaseSprintSpeed = 800;

    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void ATTCharacterPolice::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());

	if(IsValid(PC))
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if(IsValid(SubSystem))
		{
			SubSystem->AddMappingContext(PoliceCharacterIMC, 1);
		}
	}
}

void ATTCharacterPolice::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(IsValid(EnhancedInputComp))
	{
		EnhancedInputComp->BindAction(PoliceCharacterInputData->MeleeAttack, ETriggerEvent::Started, this, &ThisClass::MeleeAttack);
	}
}

float ATTCharacterPolice::GetDefaultWalkSpeed() const
{
    return BaseWalkSpeed;
}

float ATTCharacterPolice::GetSprintWalkSpeed() const
{
    return BaseSprintSpeed;
}

void ATTCharacterPolice::MeleeAttack(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Error, TEXT("Attack Mapping!"));
}
