#include "AI/TTAICharacter.h"
#include "Net/UnrealNetwork.h"
#include "TTAIController.h"
#include "Animation/AnimInstance.h"
#include "AI/TTAIDataAsset.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATTAICharacter::ATTAICharacter()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	SetReplicates(true);
	SetReplicateMovement(true);

	AIControllerClass = ATTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	bIsDead = false;
}

void ATTAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATTAICharacter, bIsDead);
	DOREPLIFETIME(ATTAICharacter, AppearanceIndex);
}

void ATTAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AIMeshDataAsset && AIMeshDataAsset->SkeltalMeshes.Num() > 0)
	{
		if (HasAuthority() && AIMeshDataAsset && AIMeshDataAsset->SkeltalMeshes.Num() > 0)
		{
			AppearanceIndex = FMath::RandHelper(AIMeshDataAsset->SkeltalMeshes.Num());
			OnRep_AppearanceIndex();
		}
	}
}

void ATTAICharacter::OnRep_AppearanceIndex()
{
	if (!GetMesh() || !AIMeshDataAsset) return;
	const auto& Arr = AIMeshDataAsset->SkeltalMeshes;
	if (!Arr.IsValidIndex(AppearanceIndex)) return;

	GetMesh()->SetSkeletalMesh(Arr[AppearanceIndex]);
}

void ATTAICharacter::EnterRagdoll()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;

	MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
	MeshComp->SetAllBodiesSimulatePhysics(true);
	MeshComp->WakeAllRigidBodies();

	bUseControllerRotationYaw = false;
}

void ATTAICharacter::OnRep_IsDead()
{
	if (bIsDead)
	{
		EnterRagdoll();
	}
}

float ATTAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority() == false) return 0.0f;

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
	}
	if (AAIController* AICon = Cast<AAIController>(Controller))
	{
		AICon->StopMovement();
	}
	if (UCapsuleComponent* Capsule = GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Die();

	return DamageAmount;
}

void ATTAICharacter::Die()
{
	if (!HasAuthority()) return;
	if (bIsDead) return;

	bIsDead = true;

	OnRep_IsDead();

	EnterRagdoll();

	SetLifeSpan(3.0f);
}
