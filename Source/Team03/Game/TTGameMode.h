#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
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

	// 경찰이 도둑을 잡았을 때 호출할 함수 (나중에 사용할꺼임)
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

private:
	// 10초 카운트다운을 1초마다 업데이트할 함수
	void UpdatePreRoundTimer();

	// 1초마다 게임 시간을 업데이트할 함수
	void UpdateGameTimer();

	// 카운트다운이 시작되었는지 여부를 저장하는 변수
	bool bIsCountdownStarted = false;

	// 역할 배정 타이머를 관리할 핸들
	FTimerHandle PreRoundTimerHandle;

	// 게임 타이머를 관리할 핸들
	FTimerHandle GameTimerHandle;

	// 독립적인 랜덤 숫자 생성기
	FRandomStream RandomStream;
};