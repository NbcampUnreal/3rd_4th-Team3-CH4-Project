#include "AI/TTAICharacter.h"
#include "Net/UnrealNetwork.h"
#include "TTAIController.h"
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

}

void ATTAICharacter::OnRep_IsDead()
{
	//죽음 몽타주
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

	SetLifeSpan(5.0f);
}
