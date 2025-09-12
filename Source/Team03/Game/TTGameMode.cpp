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
	for (TObjectPtr<APlayerState> PS : PlayerStates)
	{
		if (ATTPlayerState* TTPlayerState = Cast<ATTPlayerState>(PS))
		{
			TTPlayerState->Team = ETeam::Thief;
		}
	}

	// 2. 랜덤으로 한 명을 뽑아 '경찰'로 지정
	const int32 RandomPoliceIndex = FMath::RandRange(0, PlayerStates.Num() - 1);
	if (PlayerStates.IsValidIndex(RandomPoliceIndex))
	{
		ATTPlayerState* PolicePlayerState = Cast<ATTPlayerState>(PlayerStates[RandomPoliceIndex]);
		if (PolicePlayerState)
		{
			PolicePlayerState->Team = ETeam::Police;

			APlayerController* PoliceController = PolicePlayerState->GetPlayerController();
			if (PoliceController && PolicePawnClass)
			{
				// 기존 캐릭터(도둑)의 위치와 방향을 기억
				FVector SpawnLocation = PoliceController->GetPawn()->GetActorLocation();
				FRotator SpawnRotation = PoliceController->GetPawn()->GetActorRotation();

				// 기존 캐릭터(도둑)를 제거
				PoliceController->GetPawn()->Destroy();

				// 기억해둔 위치에 새로운 경찰 캐릭터를 스폰
				APawn* NewPolicePawn = GetWorld()->SpawnActor<APawn>(PolicePawnClass, SpawnLocation, SpawnRotation);
				if (NewPolicePawn)
				{
					// 경찰 플레이어 컨트롤러가 새로 스폰된 경찰 캐릭터에 빙의(조종 시작)
					PoliceController->Possess(NewPolicePawn);
				}
			}
		}
	}
}