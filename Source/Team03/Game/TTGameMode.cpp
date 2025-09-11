#include "Game/TTGameMode.h"
#include "Game/TTGameState.h"
#include "Game/TTPlayerState.h"
#include "Character/TTPlayerController.h"

ATTGameMode::ATTGameMode()
{
	PlayerControllerClass = ATTPlayerController::StaticClass();
	PlayerStateClass = ATTPlayerState::StaticClass();
	GameStateClass = ATTGameState::StaticClass();
}

void ATTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ATTGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	ATTGameState* const TTGameState = GetGameState<ATTGameState>();
	if (TTGameState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TTGameMode::HandleMatchHasStarted: TTGameState is not valid!"));
		return;
	}

	// GameState로부터 플레이어 목록을 가져옵니다.
	auto& PlayerStates = TTGameState->PlayerArray;

	if (PlayerStates.Num() == 0)
	{
		return;
	}

	// 모든 플레이어를 '도둑'으로 초기화
	for (APlayerState* PS : PlayerStates)
	{
		if (ATTPlayerState* TTPlayerState = Cast<ATTPlayerState>(PS))
		{
			TTPlayerState->Team = ETeam::Thief;
		}
	}

	// 랜덤으로 한 명을 뽑아 '경찰'로 변경 
	const int32 RandomPoliceIndex = FMath::RandRange(0, PlayerStates.Num() - 1);
	if (PlayerStates.IsValidIndex(RandomPoliceIndex)) // 뽑은 번호가 유효한지 한번 더 확인
	{
		if (ATTPlayerState* PolicePlayerState = Cast<ATTPlayerState>(PlayerStates[RandomPoliceIndex]))
		{
			PolicePlayerState->Team = ETeam::Police;
		}
	}
}