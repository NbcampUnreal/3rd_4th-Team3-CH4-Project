// TTPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Game/TT_Team.h"
#include "TTPlayerController.generated.h"

class UTT_WBP_HUD;

UCLASS()
class TEAM03_API ATTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;

	// 서버로 채팅 메시지를 전송하는 RPC 함수 선언
	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(const FString& Message);

	// 클라이언트로 게임 결과 UI를 전송
	UFUNCTION(Client, Reliable)
	void Client_ShowResultUI(ETeam WinningTeam);

	// 플레이어의 입력을 비활성화
	void DisablePlayerInput();

	// 플레이어의 입력을 다시 활성화
	void EnablePlayerInput();

protected:
	// 서버로부터 PlayerState를 성공적으로 받았을 때 클라이언트에서 호출
	virtual void OnRep_PlayerState() override;

private:
	// GameState의 델리게이트에 바인딩할 함수
	UFUNCTION()
	void OnChatUpdated_Handle(const TArray<FString>& Messages);

	// HUD 위젯을 생성하고 화면에 표시하는 함수
	void CreateHUD();

	// 1초마다 HUD의 타이머를 업데이트하는 함수
	void UpdateHUDTimer();

	// 에디터에서 지정할 HUD 위젯 블루프린트
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTT_WBP_HUD> HUDWidgetClass;

	// 실제 생성된 HUD 위젯의 인스턴스
	UPROPERTY()
	TObjectPtr<UTT_WBP_HUD> HUDWidgetInstance;

	// HUD 업데이트 타이머를 관리할 핸들
	FTimerHandle HUDUpdateTimerHandle;

	// 게임 결과 위젯 변수들
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PoliceWinWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PoliceLoseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ThiefWinWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ThiefLoseWidgetClass;
};