#include "AI/TTAIController.h"
#include "NavigationSystem.h"

ATTAIController::ATTAIController()
{
	MoveRadius = 1500.0f;
}

void ATTAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{

	}
}

void ATTAIController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			RandomMoveTimer,
			this,
			&ATTAIController::MoveToRandomLocation,
			5.0f,
			true,
			1.0f
		);
	}
}

void ATTAIController::MoveToRandomLocation()
{
	APawn* AIPawn = GetPawn();
	if (!AIPawn) return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return;

	FNavLocation RandomLocation;

	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(
		AIPawn->GetActorLocation(),
		MoveRadius,
		RandomLocation
	);

	if (bFoundLocation)
	{
		MoveToLocation(RandomLocation.Location);
	}
}
