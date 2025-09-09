#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TTAIController.generated.h"

UCLASS()
class TEAM03_API ATTAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATTAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

private:
	void MoveToRandomLocation();

	FTimerHandle RandomMoveTimer;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MoveRadius;
	
};
