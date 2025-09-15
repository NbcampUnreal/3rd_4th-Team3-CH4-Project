// TTPlayerController.cpp

#include "Character/TTPlayerController.h"
#include "InGameUI/TT_WBP_HUD.h"
#include "Game/TTGameState.h"
#include "TimerManager.h"

// 게임 시작 마우스 커서 숨기기 및 게임 전용 입력 모드 설정
void ATTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
// 서버 로그에 어떤 폰을 소유했는지 출력
void ATTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s"), *GetNameSafe(InPawn));
}

void ATTPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// PlayerState가 유효해지는 시점에 HUD를 생성
	CreateHUD();
}

void ATTPlayerController::CreateHUD()
{
	// 로컬 플레이어 컨트롤러인지, 그리고 HUD 위젯이 아직 생성되지 않았는지 확인
	if (IsLocalController() && HUDWidgetClass && !HUDWidgetInstance)
	{
		HUDWidgetInstance = CreateWidget<UTT_WBP_HUD>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			// 1초마다 UpdateHUDTimer 함수를 반복 호출하는 타이머를 시작합니다.
			GetWorld()->GetTimerManager().SetTimer(
				HUDUpdateTimerHandle,
				this,
				&ATTPlayerController::UpdateHUDTimer,
				1.0f,
				true
			);
		}
	}
}

void ATTPlayerController::UpdateHUDTimer()
{
	// GameState와 HUD 인스턴스가 유효한지 확인
	ATTGameState* TTGameState = GetWorld()->GetGameState<ATTGameState>();
	if (TTGameState && HUDWidgetInstance)
	{
		// GameState에서 남은 시간을 가져와 HUD 위젯의 업데이트 함수를 호출합니다.
		HUDWidgetInstance->UpdateTimer(TTGameState->RemainingTime);
	}
}