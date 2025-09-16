#include "Game/TTGameState.h"
#include "Net/UnrealNetwork.h"

ATTGameState::ATTGameState()
{
	// 초기 시간 설정 (10분)
	RemainingTime = 60;

	// 카운트다운 시간 초기값 설정 (0으로 시작)
	RoleAssignmentCountdownTime = 0;
}

void ATTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 변수를 모든 클라이언트에게 복제하도록 설정
	DOREPLIFETIME(ATTGameState, RemainingTime);
	DOREPLIFETIME(ATTGameState, RoleAssignmentCountdownTime);
	DOREPLIFETIME(ATTGameState, ChatMessages);
}

void ATTGameState::AddChatMessage(const FString& Message)
{
	// 서버에서만 이 함수가 호출
	if (HasAuthority())
	{
		ChatMessages.Add(Message);
		// 클라이언트에서도 OnRep 함수가 호출되도록 수동으로 호출 (서버 자신을 위해)
		OnRep_ChatMessages();
	}
}

void ATTGameState::OnRep_ChatMessages()
{
	// 배열이 업데이트되었음을 UI에게 방송(Broadcast)
	OnChatUpdated.Broadcast(ChatMessages);
}