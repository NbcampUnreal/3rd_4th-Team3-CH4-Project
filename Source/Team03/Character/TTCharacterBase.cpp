// TTCharacterBase.cpp

#include "Character/TTCharacterBase.h"
#include "OutGameUI/TTGameUserSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/TTBaseCharacterInput.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/TTWeaponBase.h"

ATTCharacterBase::ATTCharacterBase()
{
	// 초기 값 설정
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	bReplicates = true;

	// 스켈레탈 메시 및 캡슐 초기 값 설정
	float CharacterRadius = 40.f;
	float CharacterHalfHeight = 90.f;

	GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);
	FVector PivotLocation(0.f, 0.f, -CharacterHalfHeight);
	FRotator PivotRotation(0.f, -90.f, 0.f);
	GetMesh()->SetRelativeLocationAndRotation(PivotLocation, PivotRotation);

	// 캐릭터 무브먼트 관련 수치 조정
	BaseWalkSpeed = 300;
	BaseSprintSpeed = 600;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = GetDefaultWalkSpeed();

	// 스프링 암 컴포넌트 초기 설정
	SprintArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SprintArmComp->SetupAttachment(RootComponent);
	SprintArmComp->bUsePawnControlRotation = true;
	SprintArmComp->TargetArmLength = 400.0f;

	// 카메라 컴포넌트 초기 설정
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SprintArmComp);
	CameraComp->bUsePawnControlRotation = false;
}

void ATTCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// 게임모드에서 적절한 플레이어 컨트롤러를 연결할 수 있도록 추후 수정 부분
	APlayerController* PC = Cast<APlayerController>(GetController());

	if(IsValid(PC))
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if(IsValid(SubSystem))
		{
			SubSystem->AddMappingContext(BaseCharcterIMC, 0);
		}
	}

	if(DefaultWeaponClass)
	{
		// 기본무기를 현재무기에 스폰
		CurrentWeapon = GetWorld()->SpawnActor<ATTWeaponBase>(DefaultWeaponClass);

		if(CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(
				GetMesh(),
				FAttachmentTransformRules::SnapToTargetIncludingScale,
				FName("MeleeWeapon")
			);
		}
	}
}

void ATTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(IsValid(EnhancedInputComp))
	{
		EnhancedInputComp->BindAction(BaseCharacterInputData->Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComp->BindAction(BaseCharacterInputData->Jump, ETriggerEvent::Triggered, this, &ThisClass::StartJump);
		EnhancedInputComp->BindAction(BaseCharacterInputData->Jump, ETriggerEvent::Completed, this, &ThisClass::StopJump);
		EnhancedInputComp->BindAction(BaseCharacterInputData->Sprint, ETriggerEvent::Triggered, this, &ThisClass::StartSprint);
		EnhancedInputComp->BindAction(BaseCharacterInputData->Sprint, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		EnhancedInputComp->BindAction(BaseCharacterInputData->Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
	}
}

float ATTCharacterBase::GetDefaultWalkSpeed() const
{
	return BaseWalkSpeed;
}

float ATTCharacterBase::GetSprintWalkSpeed() const
{
	return BaseSprintSpeed;
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
	
	// 만든 GetTTGameUserSettings() 함수를 통해 설정 객체 가져옴
	UTTGameUserSettings* UserSettings = UTTGameUserSettings::GetTTGameUserSettings();
	if (UserSettings)
	{
		// 저장된 마우스 감도 값을 가져와서 마우스 입력 값에 곱해준다
		const float MouseSensitivity = UserSettings->GetMouseSensitivity();
		AddControllerYawInput(LookInput.X * MouseSensitivity);
		AddControllerPitchInput(LookInput.Y * MouseSensitivity);
	}
	// 만약 설정 객체를 가져오는 데 실패하면, 기본값으로 작동
	else 
	{
		AddControllerYawInput(LookInput.X);
		AddControllerPitchInput(LookInput.Y);
	}
}

void ATTCharacterBase::StartSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement())
	{
		if(HasAuthority())
		{
			bIsSprinting = true;
			GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? GetSprintWalkSpeed() : GetDefaultWalkSpeed();
		}
		else
		{
			ServerStartSprint();
		}
	}
}
void ATTCharacterBase::StopSprint(const FInputActionValue& Value)
{
	if(GetCharacterMovement())
	{
		if(HasAuthority())
		{
			bIsSprinting = false;
			GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? GetSprintWalkSpeed() : GetDefaultWalkSpeed();
		}
		else
		{
			ServerStopSprint();
		}
	}
}

void ATTCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsSprinting);
}

void ATTCharacterBase::OnRep_ChangeSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? GetSprintWalkSpeed() : GetDefaultWalkSpeed();
}

void ATTCharacterBase::ServerStartSprint_Implementation()
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? GetSprintWalkSpeed() : GetDefaultWalkSpeed();
}

void ATTCharacterBase::ServerStopSprint_Implementation()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? GetSprintWalkSpeed() : GetDefaultWalkSpeed();
}