#include "AI/TTAICharacter.h"
#include "Net/UnrealNetwork.h"
#include "TTAIController.h"
#include "Animation/AnimInstance.h"
#include "AI/TTAIDataAsset.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"

ATTAICharacter::ATTAICharacter()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	SetReplicates(true);
	SetReplicateMovement(true);

	AIControllerClass = ATTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bIsDead = false;
}

void ATTAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATTAICharacter, bIsDead);
}

void ATTAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AIMeshDataAsset && AIMeshDataAsset->SkeltalMeshes.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, AIMeshDataAsset->SkeltalMeshes.Num() - 1);
		GetMesh()->SetSkeletalMesh(AIMeshDataAsset->SkeltalMeshes[Index]);
	}

}

void ATTAICharacter::OnRep_IsDead()
{
	if (bIsDead)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			if (DeathMontage)
			{
				AnimInstance->Montage_Play(DeathMontage, 1.0f);
			}
		}
	}
}

float ATTAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority() == false) return 0.0f;

	Die();

	return DamageAmount;
}

void ATTAICharacter::Die()
{
	if (!HasAuthority()) return;
	if (bIsDead) return;

	bIsDead = true;

	OnRep_IsDead();

	SetLifeSpan(5.0f);
}
