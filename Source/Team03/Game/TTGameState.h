#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TTGameState.generated.h"


UCLASS()
class TEAM03_API ATTGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ATTGameState();

	// 게임의 남은 시간 변수 (10분)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category = "Game")
	int32 RemainingTime;

	// 역할 배정 카운트다운 시간 변수 (10초)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category = "Game")
	int32 RoleAssignmentCountdownTime;

	// 변수 복제를 위한 필수 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
