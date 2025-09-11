#include "Game/TTPlayerState.h"
#include "Net/UnrealNetwork.h"

ATTPlayerState::ATTPlayerState()
{
	Team = ETeam::None;
}

void ATTPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Team 변수를 모든 클라이언트에게 복제하도록 설정
	DOREPLIFETIME(ATTPlayerState, Team);
}

void ATTPlayerState::OnRep_Team()
{
	// 팀 정보가 변경되었음을 알림 (UI 업데이트 트리거로 사용)
	OnTeamChanged.Broadcast(Team);
}