#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Game/TT_Team.h"
#include "TTGameMode.generated.h"

// .h 파일에서는 class 키워드로 전방 선언하여 헤더 파일을 가볍게 유지합니다.
class ATTGameState;

UCLASS()
class TEAM03_API ATTGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATTGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 플레이어가 이탈했을 때 호출되는 함수
	virtual void Logout(AController* Exiting) override;

	// 경찰이 도둑을 잡았을 때 호출할 함수
	void OnThiefCaught();

protected:
	// 경찰 캐릭터로 사용할 블루프린트 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<APawn> PolicePawnClass;

	// 경기를 시작하기 위한 목표 플레이어 수
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 NumPlayersToStartMatch = 3;

	// 경기 시작까지의 딜레이 시간 - 기본값을 20초로 설정
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float MatchStartDelay = 20.0f;

	// 게임 종료 후 로비로 돌아가기까지의 딜레이 시간
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	float ReturnToLobbyDelay = 10.0f;

private:
	// 포지션 배정 카운트다운을 1초마다 업데이트할 함수
	void UpdatePreRoundTimer();

	// 1초마다 게임 시간을 업데이트할 함수
	void UpdateGameTimer();

	// 모든 플레이어를 텔레포트시키고 입력을 막는 함수
	void TeleportAndImmobilizePlayers();

	// 모든 플레이어의 입력을 다시 활성화하는 함수
	void EnableAllPlayerInputs();

	// 게임이 끝났을 때 호출되는 함수
	void EndGame(ETeam Winner);

	// 모든 플레이어를 로비로 돌려보내는 함수
	void ReturnToLobby();

	// 카운트다운이 시작되었는지 여부를 저장하는 변수
	bool bIsCountdownStarted = false;

	// 역할 배정 타이머를 관리할 핸들
	FTimerHandle PreRoundTimerHandle;

	// 게임 타이머를 관리할 핸들
	FTimerHandle GameTimerHandle;

	// 로비로 돌아가는 타이머를 관리할 핸들
	FTimerHandle ReturnToLobbyTimerHandle;

	// 독립적인 랜덤 숫자 생성기
	FRandomStream RandomStream;

	// 세션 플레이어 수가 변경될 때마다 호출되는 함수
	void UpdateSessionPlayerCount();
};