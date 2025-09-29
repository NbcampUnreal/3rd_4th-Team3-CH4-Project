// TTCharacterPolice.cpp

#include "Character/TTCharacterPolice.h"
#include "Input/TTPoliceInput.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameStateBase.h"
#include "Weapon/TTWeaponBase.h"
#include "Component/TTBaseStatComponent.h"

ATTCharacterPolice::ATTCharacterPolice():
	MeleeAttackMontagePlayTime(0.f),
	bCanAttack(1),
	LastStartMeleeAttackTime(0.f),
	MeleeAttackTimeDifference(0.f),
	MinAllowedTimeForMeleeAttack(0.02f)
{
	bReplicates = true;

    // 경찰 캐릭터 무브먼트 관련 수치 조정
    BaseWalkSpeed = 500;
    BaseSprintSpeed = 800;

    GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	BaseStatComp = CreateDefaultSubobject<UTTBaseStatComponent>(TEXT("BaseStatComp"));
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
			// 경찰 IMC 1번 슬롯 연결
			SubSystem->AddMappingContext(PoliceCharacterIMC, 1);
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

	if(IsValid(MeleeAttackMontage))
	{
		MeleeAttackMontagePlayTime = MeleeAttackMontage->GetPlayLength()/2;
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
	if(bCanAttack && !GetCharacterMovement()->IsFalling())
	{
		ServerRPCMeleeAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());

		// 서버가 아닌 로컬 클라이언트에서만 애니메이션 재생
		if(HasAuthority() == false && IsLocallyControlled() == true)
		{
			PlayMeleeAttackMontage();
		}
	}
}

// 데미지 처리 함수
float ATTCharacterPolice::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if(BaseStatComp && DamageAmount > 0)
	{
		BaseStatComp->ApplyDamage(DamageAmount);
	}

	// 캐릭터의 체력이 0 이하이고 아직 죽지 않은 상태일때
	if(BaseStatComp->GetCurrentHP() <= KINDA_SMALL_NUMBER && !bIsDead)
	{
		// 죽음 처리
		bIsDead = 1;

		ActivateRagdoll();
	}

	return DamageAmount;
}

// 근접 공격 히트 체크 함수
void ATTCharacterPolice::CheckMeleeAttackHit()
{
	if(IsLocallyControlled() == true)
	{
		TArray<FHitResult> OutHitResults;
		TSet<ACharacter*> DamagedCharacters;
		FCollisionQueryParams Params(NAME_None, false, this);

		// 근접 공격 범위, 반경
		const float MeleeAttackRange = 50.f;
		const float MeleeAttackRadius = 50.f;
		
		// 근접 공격 시작, 종료 지점
		const FVector Forward = GetActorForwardVector();
		const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + GetActorForwardVector() * MeleeAttackRange;

		// 구체 형태의 충돌 체크
		bool bIsHitDetected = GetWorld()->SweepMultiByChannel(
			OutHitResults,
			Start,
			End,
			FQuat::Identity,
			ECC_Camera,
			FCollisionShape::MakeSphere(MeleeAttackRadius),
			Params
		);

		// OutHitResults 배열에 충돌된 액터들 중 ACharacter 타입만 필터링하여 데미지 적용
		if(!OutHitResults.IsEmpty())
		{
			for(auto const& OutHitResult : OutHitResults)
			{
				ACharacter* DamagedCharacter = Cast<ACharacter>(OutHitResult.GetActor());
				if(IsValid(DamagedCharacter) == true)
				{
					DamagedCharacters.Add(DamagedCharacter);
				}
			}
			
			FDamageEvent DamageEvent;
			for(auto const& DamagedCharacter : DamagedCharacters)
			{
				// 한 번에 한 캐릭터에게만 데미지 적용
				ServerRPCPerformMeleeHit(DamagedCharacter, GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
			}
		}

		// 아무도 맞추지 못했을 때 자기 자신에게 데미지를 입히는 부분
		if(!bIsHitDetected)
		{
			ServerRPCApplySelfDamage(33.4f);
		}

		FColor DrawColor = bIsHitDetected ? FColor::Green : FColor::Red;
		DrawDebugMeleeAttack(DrawColor, Start, End, Forward);
	}
}

// 근접 공격 디버그 드로잉 함수
void ATTCharacterPolice::DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = MeleeAttackRange * 0.5f;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, MeleeAttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
}

// 근접 공격 애니메이션 몽타주 재생 함수
void ATTCharacterPolice::PlayMeleeAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance) == true)
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(MeleeAttackMontage);
	}
}

// 네트워크 복제 설정 함수
void ATTCharacterPolice::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bCanAttack);
	DOREPLIFETIME(ThisClass, bIsDead);
}

// bCanAttack 변수 변경시 호출되는 함수
void ATTCharacterPolice::OnRep_CanAttack()
{
	if(bCanAttack == true)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}

// 모든 클라이언트에서 근접 공격 애니메이션 재생 및 공격 쿨타임 시작
void ATTCharacterPolice::MulticastRPCMeleeAttack_Implementation()
{
	if(HasAuthority() == false && IsLocallyControlled() == false)
	{
		PlayMeleeAttackMontage();
	}
}

// 서버 RPCMeleeAttack 유효성 검사 함수
bool ATTCharacterPolice::ServerRPCMeleeAttack_Validate(float InStartMeleeAttackTime)
{
	if(LastStartMeleeAttackTime == 0.0f)
	{
		// 최초 공격의 경우 무조건 허용
		return true;
	}

	// 밀리 공격 방지: 이전 공격 기준으로 너무 빠르게 공격 시도했는지 체크
	return (MeleeAttackMontagePlayTime - 0.1f) < (InStartMeleeAttackTime - LastStartMeleeAttackTime);
}

// 서버 (서버 딜레이를 고려한 공격 속도 설정 함수)
void ATTCharacterPolice::ServerRPCMeleeAttack_Implementation(float InStartMeleeAttackTime)
{
	// 서버딜레이 = 현재서버시간 - 공격입력이들어왔을때서버시간
	MeleeAttackTimeDifference = GetWorld()->GetTimeSeconds() - InStartMeleeAttackTime;
	// Clamp로 0 ~ 근접 공격 몽타주 재생시간 사이로 제한
	MeleeAttackTimeDifference = FMath::Clamp(MeleeAttackTimeDifference, 0.f, MeleeAttackMontagePlayTime);

	if(KINDA_SMALL_NUMBER < MeleeAttackMontagePlayTime - MeleeAttackTimeDifference)
	{

		bCanAttack = false;
		OnRep_CanAttack();

		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			// CreateLambda를 통해 람다를 타이머 델리게이트로 변환
			FTimerDelegate::CreateLambda([&]()
			{
				bCanAttack = true;
				OnRep_CanAttack();
			}),
			MeleeAttackMontagePlayTime - MeleeAttackTimeDifference,
			false,
			-1.f
		);
	}

	LastStartMeleeAttackTime = InStartMeleeAttackTime;
	PlayMeleeAttackMontage();

	MulticastRPCMeleeAttack();
}

// 서버 RPCPerformMeleeHit 유효성 검사 함수
bool ATTCharacterPolice::ServerRPCPerformMeleeHit_Validate(ACharacter* InDamagedCharacters, float InCheckTime)
{
	return MinAllowedTimeForMeleeAttack < (InCheckTime - LastStartMeleeAttackTime);
}

// 서버 (특정 캐릭터에게 데미지를 입히는 함수)
void ATTCharacterPolice::ServerRPCPerformMeleeHit_Implementation(ACharacter* InDamagedCharacters, float InCheckTime)
{
	if(IsValid(InDamagedCharacters) == true)
	{
		const float MeleeAttackDamage = 50.f;
		FDamageEvent DamageEvent;
		InDamagedCharacters->TakeDamage(MeleeAttackDamage, DamageEvent, GetController(), this);
	}
}

// 서버 (본인에게 데미지를 입히는 패널티 함수)
void ATTCharacterPolice::ServerRPCApplySelfDamage_Implementation(float DamageAmount)
{
	FDamageEvent DamageEvent;
	TakeDamage(DamageAmount, DamageEvent, GetController(), this);
}
