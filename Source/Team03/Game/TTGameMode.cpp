#include "Game/TTGameMode.h"
#include "Game/TTGameState.h"
#include "Game/TTPlayerState.h"
#include "Character/TTPlayerController.h"
#include "TimerManager.h"

ATTGameMode::ATTGameMode()
{
	PlayerControllerClass = ATTPlayerController::StaticClass();
	PlayerStateClass = ATTPlayerState::StaticClass();
	GameStateClass = ATTGameState::StaticClass();

	RandomStream.Initialize(FDateTime::Now().GetTicks());
}

void ATTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ATTGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	// 10초 카운트다운 시작
	// MatchStartDelay(10초) 후에 AssignRolesAfterDelay 함수를 호출하도록 타이머를 설정
	FTimerHandle MatchStartTimer;
	GetWorld()->GetTimerManager().SetTimer(
		MatchStartTimer,
		this,
		&ATTGameMode::AssignRolesAfterDelay,
		MatchStartDelay,
		false
	);

	// (나중에 할 일) 이 시점에서 모든 플레이어의 UI에 "10초 후 게임이 시작됩니다" 라는 메시지를 띄워줄 수 있음
}

void ATTGameMode::AssignRolesAfterDelay()
{
	// 10초 후 역할 배정 시작
	// 이 함수는 10초 후에 정확히 한 번만 실행됩니다.

	ATTGameState* const TTGameState = GetGameState<ATTGameState>();
	if (TTGameState == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("AssignRolesAfterDelay: TTGameState is not valid!"));
		return;
	}

	auto& PlayerStates = TTGameState->PlayerArray;
	if (PlayerStates.Num() == 0) return;

	// 모든 플레이어를 '도둑'으로 초기화
	for (TObjectPtr<APlayerState> PS : PlayerStates)
	{
		if (ATTPlayerState* TTPlayerState = Cast<ATTPlayerState>(PS))
		{
			TTPlayerState->Team = ETeam::Thief;
		}
	}

	// 랜덤으로 한 명을 뽑아 '경찰'로 변경
	const int32 RandomPoliceIndex = RandomStream.RandRange(0, PlayerStates.Num() - 1);
	if (PlayerStates.IsValidIndex(RandomPoliceIndex))
	{
		if (ATTPlayerState* PolicePlayerState = Cast<ATTPlayerState>(PlayerStates[RandomPoliceIndex]))
		{
			PolicePlayerState->Team = ETeam::Police;

			// 경찰 캐릭터 교체 로직
			APlayerController* PoliceController = PolicePlayerState->GetPlayerController();
			if (PoliceController && PolicePawnClass)
			{
				FVector SpawnLocation = PoliceController->GetPawn()->GetActorLocation();
				FRotator SpawnRotation = PoliceController->GetPawn()->GetActorRotation();
				PoliceController->GetPawn()->Destroy();
				APawn* NewPolicePawn = GetWorld()->SpawnActor<APawn>(PolicePawnClass, SpawnLocation, SpawnRotation);
				if (NewPolicePawn)
				{
					PoliceController->Possess(NewPolicePawn);
				}
			}
		}
	}

	// 역할 배정이 끝나면 1초마다 반복하는 게임 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		GameTimerHandle,
		this,
		&ATTGameMode::UpdateGameTimer,
		1.0f,
		true
	);
}

void ATTGameMode::UpdateGameTimer()
{
	ATTGameState* const TTGameState = GetGameState<ATTGameState>();
	if (TTGameState)
	{
		// 남은 시간을 1초 줄임
		TTGameState->RemainingTime--;

		// 시간이 다 되었다면
		if (TTGameState->RemainingTime <= 0)
		{
			// 타이머를 멈춥니다.
			GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);

			// 도둑 승리
			UE_LOG(LogTemp, Warning, TEXT("Game Over: Thieves Win!"));
			// 여기에 게임 종료 및 결과 UI 표시 로직을 추가
		}
	}
}

void ATTGameMode::OnThiefCaught()
{
	ATTGameState* const TTGameState = GetGameState<ATTGameState>();
	if (TTGameState == nullptr) return;

	int32 ThievesRemaining = 0;
	// 현재 게임에 있는 모든 플레이어를 확인
	for (TObjectPtr<APlayerState> PS : TTGameState->PlayerArray)
	{
		ATTPlayerState* TTPlayerState = Cast<ATTPlayerState>(PS);
		// 아직 '도둑' 팀이고 '죽지 않은' 상태의 플레이어 수를 체크 (지금은 IsDead 같은 상태가 없으므로 팀만 체크)
		if (TTPlayerState && TTPlayerState->Team == ETeam::Thief)
		{
			ThievesRemaining++;
		}
	}

	// 만약 남은 도둑이 없다면
	if (ThievesRemaining == 0)
	{
		// 타이머 스톱
		GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);

		// 경찰 승리 로그
		UE_LOG(LogTemp, Warning, TEXT("Game Over: Police Wins!"));
		// 여기에 게임 종료 및 결과 UI 표시 로직을 추가하면 됩니다.
	}
}