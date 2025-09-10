// TTCharacterBase.cpp

#include "Character/TTCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/TTBaseCharacterInput.h"

ATTCharacterBase::ATTCharacterBase()
{
	BaseWalkSpeed = 300;
	BaseSprintSpeed = 600;

	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	SprintArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SprintArmComp->SetupAttachment(RootComponent);
	SprintArmComp->bUsePawnControlRotation = true;
	SprintArmComp->TargetArmLength = 400.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SprintArmComp);
	CameraComp->bUsePawnControlRotation = false;
}

void ATTCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetController());

	if(IsValid(PC))
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if(IsValid(SubSystem))
		{
			SubSystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
		}
	}
}

void ATTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(IsValid(EnhancedInputComp))
	{
		EnhancedInputComp->BindAction(PlayerBaseCharacterInputConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComp->BindAction(PlayerBaseCharacterInputConfig->Jump, ETriggerEvent::Triggered, this, &ThisClass::StartJump);
		EnhancedInputComp->BindAction(PlayerBaseCharacterInputConfig->Jump, ETriggerEvent::Completed, this, &ThisClass::StopJump);
		EnhancedInputComp->BindAction(PlayerBaseCharacterInputConfig->Sprint, ETriggerEvent::Triggered, this, &ThisClass::StartSprint);
		EnhancedInputComp->BindAction(PlayerBaseCharacterInputConfig->Sprint, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		EnhancedInputComp->BindAction(PlayerBaseCharacterInputConfig->Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}
}

void ATTCharacterBase::Move(const FInputActionValue& Value)
{
	const FVector2D InMovementVector = Value.Get<FVector2D>();

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator ControlYawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InMovementVector.X);
	AddMovementInput(RightDirection, InMovementVector.Y);
}

void ATTCharacterBase::StartJump(const FInputActionValue& Value)
{
	if(Value.Get<bool>())
	{
		Jump();
	}
}

void ATTCharacterBase::StopJump(const FInputActionValue& Value)
{
	if(!Value.Get<bool>())
	{
		StopJumping();
	}
}

void ATTCharacterBase::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ATTCharacterBase::StartSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement())
	{
		bIsSprinting = true;
		UpdateSpeed();
	}
}
void ATTCharacterBase::StopSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement())
	{
		bIsSprinting = false;
		UpdateSpeed();
	}
}

void ATTCharacterBase::UpdateSpeed()
{
	float CurrentBaseSpeed = bIsSprinting ? BaseSprintSpeed : BaseWalkSpeed;

	GetCharacterMovement()->MaxWalkSpeed = CurrentBaseSpeed;
}
