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

ATTCharacterPolice::ATTCharacterPolice():
	bCanAttack(true),
	MeleeAttackMontagePlayTime(0.f)
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
			// 경찰 IMC 1번 슬롯 연결
			SubSystem->AddMappingContext(PoliceCharacterIMC, 1);
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
	// && !GetCharacterMovement()->IsFalling()
	if(bCanAttack)
	{
		ServerRPCMeleeAttack();
	}
}

float ATTCharacterPolice::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("TakeDamage: %f"), DamageAmount), true, true, FLinearColor::Green, 5.f);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ATTCharacterPolice::CheckMeleeAttackHit()
{
	if(HasAuthority())
	{
		TArray<FHitResult> OutHitResults;
		TSet<ACharacter*> DamagedCharacters;
		FCollisionQueryParams Params(NAME_None, false, this);

		const float MeleeAttackRange = 50.f;
		const float MeleeAttackRadius = 50.f;
		const float MeleeAttackDamage = 10.f;
		const FVector Forward = GetActorForwardVector();
		const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + GetActorForwardVector() * MeleeAttackRange;

		bool bIsHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, ECC_Camera, FCollisionShape::MakeSphere(MeleeAttackRadius), Params);
		if(bIsHitDetected == true)
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
				DamagedCharacter->TakeDamage(MeleeAttackDamage, DamageEvent, GetController(), this);
			}
		}

		FColor DrawColor = bIsHitDetected ? FColor::Green : FColor::Red;
		DrawDebugMeleeAttack(DrawColor, Start, End, Forward);
	}
}

void ATTCharacterPolice::DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = MeleeAttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, MeleeAttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
}

void ATTCharacterPolice::PlayMeleeAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(IsValid(AnimInstance) == true)
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(MeleeAttackMontage);
	}
}

void ATTCharacterPolice::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bCanAttack);
}

void ATTCharacterPolice::OnRep_CanAttack()
{
	if(bCanAttack == true)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	else
	{
		//GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}

void ATTCharacterPolice::MulticastRPCMeleeAttack_Implementation()
{
	if(HasAuthority() == true)
	{
		bCanAttack = false;

		OnRep_CanAttack();

		FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([&]()
											   {
												   bCanAttack = true;
												   OnRep_CanAttack();
											   }), MeleeAttackMontagePlayTime, false);
	}

	PlayMeleeAttackMontage();
}

bool ATTCharacterPolice::ServerRPCMeleeAttack_Validate()
{
	return true;
}

void ATTCharacterPolice::ServerRPCMeleeAttack_Implementation()
{
	MulticastRPCMeleeAttack();
}
