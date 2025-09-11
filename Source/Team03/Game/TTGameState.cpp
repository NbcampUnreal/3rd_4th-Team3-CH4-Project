#include "Game/TTGameState.h"
#include "Net/UnrealNetwork.h"

ATTGameState::ATTGameState()
{
	// 초기 시간 설정 (10분)
	RemainingTime = 600;
}

void ATTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// RemainingTime 변수를 모든 클라이언트에게 복제하도록 설정
	DOREPLIFETIME(ATTGameState, RemainingTime);
}