#include "Game/TTGameMode.h"
#include "Game/TTGameState.h"
#include "Game/TTPlayerState.h"
#include "Character/TTPlayerController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"

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

	if (!bIsCountdownStarted && GetNumPlayers() >= NumPlayersToStartMatch)
	{
		// 카운트다운이 시작되었다고 표시 (다른 플레이어가 접속해도 중복 실행X)
		bIsCountdownStarted = true;

		// GameState의 카운트다운 시간을 초기화하고 10초 타이머를 시작
		ATTGameState* const TTGameState = GetGameState<ATTGameState>();
		if (TTGameState)
		{
			TTGameState->RoleAssignmentCountdownTime = MatchStartDelay;

			GetWorld()->GetTimerManager().SetTimer(
				PreRoundTimerHandle,
				this,
				&ATTGameMode::UpdatePreRoundTimer,
				1.0f,
				true
			);
		}
	}
}

void ATTGameMode::UpdatePreRoundTimer()
{
	ATTGameState* const TTGameState = GetGameState<ATTGameState>();
	if (TTGameState)
	{
		// 카운트다운 시간을 1초 줄임
		TTGameState->RoleAssignmentCountdownTime--;

		// 카운트다운 5초 남았을 때 랜덤 위치 텔레포트
		if (TTGameState->RoleAssignmentCountdownTime == 5)
		{
			TeleportAndImmobilizePlayers();
		}

		// 카운트다운이 끝나면
		if (TTGameState->RoleAssignmentCountdownTime <= 0)
		{
			// 이 타이머를 멈추고 역할 배정을 시작
			GetWorld()->GetTimerManager().ClearTimer(PreRoundTimerHandle);
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
			// 역할 배정이 끝나면 플레이어의 입력을 다시 풀어줌
			EnableAllPlayerInputs();

			// 역할 배정이 끝나면 메인 게임 타이머 시작
			GetWorld()->GetTimerManager().SetTimer(GameTimerHandle, this, &ATTGameMode::UpdateGameTimer, 1.0f, true);
		}
	}
}

void ATTGameMode::TeleportAndImmobilizePlayers()
{
	// 맵에 배치된 모든 TargetPoint를 찾아서 배열에 담음
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), SpawnPoints);

	// 스폰 포인트가 없으면 행동 X
	if (SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("스폰 포인트가 없습니다."));
		return;
	}

	ATTGameState* const TTGameState = GetGameState<ATTGameState>();
	if (TTGameState == nullptr) return;

	// 모든 플레이어를 순회하며 텔레포트시키고 입력을 막기
	for (int32 i = 0; i < TTGameState->PlayerArray.Num(); ++i)
	{
		ATTPlayerController* PC = Cast<ATTPlayerController>(TTGameState->PlayerArray[i]->GetPlayerController());
		if (PC)
		{
			// 플레이어의 입력을 막기
			PC->DisablePlayerInput();

			APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				// 스폰 포인트를 순환하며 위치를 지정하고 텔레포트
				int32 SpawnPointIndex = i % SpawnPoints.Num();
				FVector NewLocation = SpawnPoints[SpawnPointIndex]->GetActorLocation();
				Pawn->SetActorLocation(NewLocation);
			}
		}
	}
}

void ATTGameMode::EnableAllPlayerInputs()
{
	ATTGameState* const TTGameState = GetGameState<ATTGameState>();
	if (TTGameState == nullptr) return;

	// 모든 플레이어를 순회하며 입력을 다시 활성화합니다.
	for (TObjectPtr<APlayerState> PS : TTGameState->PlayerArray)
	{
		ATTPlayerController* PC = Cast<ATTPlayerController>(PS->GetPlayerController());
		if (PC)
		{
			PC->EnablePlayerInput();
		}
	}
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
			// 도둑 승리
			EndGame(ETeam::Thief);
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
		EndGame(ETeam::Police);
	}
}

void ATTGameMode::EndGame(ETeam Winner)
{
	ATTGameState* const TTGameState = GetGameState<ATTGameState>();
	if (TTGameState)
	{
		// GameState에 승리 팀을 기록 (모든 클라이언트로 전파)
		TTGameState->WinningTeam = Winner;
	}

	// 모든 타이머 스탑
	GetWorld()->GetTimerManager().ClearTimer(GameTimerHandle);

	// 10초 후에 ReturnToLobby 함수를 호출하는 타이머
	GetWorld()->GetTimerManager().SetTimer(
		ReturnToLobbyTimerHandle,
		this,
		&ATTGameMode::ReturnToLobby,
		ReturnToLobbyDelay,
		false
	);

	UE_LOG(LogTemp, Warning, TEXT("Game Over! Winner: %s"), *(UEnum::GetValueAsString(Winner)));

	// AGameMode의 EndMatch() 함수를 호출하여 경기를 공식적으로 종료
	// EndMatch()는 모든 플레이어의 입력을 막는 등의 처리를 도와줌
	Super::EndMatch();
}

void ATTGameMode::ReturnToLobby()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// 세션을 파괴하고 모든 플레이어를 메인 메뉴 맵으로 보냄
		// bSeamless (심리스 여부)는 false로 설정하여 로딩 화면이 뜨도록 합니다.
		World->ServerTravel("/Game/TT/Maps/OutGameUI/MainMenu_Map", false, false);
	}
}