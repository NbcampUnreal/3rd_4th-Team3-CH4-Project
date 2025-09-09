#include "AI/TTAICharacter.h"
#include "Net/UnrealNetwork.h"
#include "TTAIController.h"

ATTAICharacter::ATTAICharacter()
{
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

float ATTAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority() == false) return 0.0f;

	Die();

	return 0.0f;
}

void ATTAICharacter::Die()
{
	bIsDead = true;

	SetLifeSpan(5.0f);
}
