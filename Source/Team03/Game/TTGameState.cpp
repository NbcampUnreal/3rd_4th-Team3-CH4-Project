#include "Game/TTGameState.h"
#include "Net/UnrealNetwork.h"

ATTGameState::ATTGameState()
{
	// 초기 시간 설정 (10분)
	RemainingTime = 600;

	// 카운트다운 시간 초기값 설정 (0으로 시작)
	RoleAssignmentCountdownTime = 0;
}

void ATTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 변수를 모든 클라이언트에게 복제하도록 설정
	DOREPLIFETIME(ATTGameState, RemainingTime);
	DOREPLIFETIME(ATTGameState, RoleAssignmentCountdownTime);
}