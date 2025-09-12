// TTPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TTPlayerController.generated.h"

UCLASS()
class TEAM03_API ATTPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public: 
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
};
