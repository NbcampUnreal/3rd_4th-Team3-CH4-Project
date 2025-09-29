// TTCharacterThief.cpp

#include "Character/TTCharacterThief.h"
#include "Character/TTSpectatorPawn.h"
#include "Character/TTPlayerController.h"
#include "Character/DataAsset/TTCharacterThiefData.h"
#include "Game/TTGameMode.h"
#include "Component/TTBaseStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputSubsystems.h"

ATTCharacterThief::ATTCharacterThief():
	Index(-1)
{
	bReplicates = true;

	BaseStatComp = CreateDefaultSubobject<UTTBaseStatComponent>(TEXT("BaseStatComp"));
}

void ATTCharacterThief::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		if(ThiefMeshData && ThiefMeshData->SkeltalMeshes.Num() > 0)
		{
			Index = FMath::RandRange(0, ThiefMeshData->SkeltalMeshes.Num()-1);
		}
	}
}

void ATTCharacterThief::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Index);
	DOREPLIFETIME(ThisClass, bIsDead);
}

float ATTCharacterThief::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if(BaseStatComp && ActualDamage > 0)
	{
		BaseStatComp->ApplyDamage(ActualDamage);
	}

	// 캐릭터의 체력이 0 이하이고 아직 죽지 않은 상태일때
	if(BaseStatComp->GetCurrentHP() <= KINDA_SMALL_NUMBER && !bIsDead)
	{
		// 죽음 처리
		bIsDead = true;

		if(HasAuthority())
		{
			// Spectator Pawn으로 변경하는 부분
			if(ATTPlayerController* PC = Cast<ATTPlayerController>(GetController()))
			{
				// Spectator Pawn 생성
				if(SpectatorPawnClass)
				{
					ATTSpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ATTSpectatorPawn>(
						SpectatorPawnClass,
						GetActorLocation(),
						GetActorRotation()
					);

					if(SpectatorPawn)
					{
						PC->Possess(SpectatorPawn);
					}
				}
			}
		}

		ActivateRagdoll();

		if (HasAuthority())
		{
			// 현재 월드의 게임모드 가져오기
			ATTGameMode* GameMode = GetWorld()->GetAuthGameMode<ATTGameMode>();
			if (GameMode)
			{
				// 게임모드에 OnThiefCaught 함수 호출 (잡힌 도둑 체크)
				GameMode->OnThiefCaught();
			}
		}
	}

	return ActualDamage;
}

void ATTCharacterThief::OnRep_SelectMesh()
{
	GetMesh()->SetSkeletalMesh(ThiefMeshData->SkeltalMeshes[Index]);
}
