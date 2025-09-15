// TTPlayerController.cpp

#include "Character/TTPlayerController.h"
#include "InGameUI/TT_WBP_HUD.h"
#include "Game/TTGameState.h"
#include "Game/TTPlayerState.h"
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

	ATTGameState* TTGameState = GetWorld()->GetGameState<ATTGameState>();
	if (TTGameState && IsLocalController())
	{
		// OnChatUpdated 신호가 오면, OnChatUpdated_Handle 함수를 실행하도록 구독
		TTGameState->OnChatUpdated.AddUObject(this, &ATTPlayerController::OnChatUpdated_Handle);
	}
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
	ATTGameState* TTGameState = GetWorld()->GetGameState<ATTGameState>();
	if (TTGameState && HUDWidgetInstance)
	{
		// 만약 역할 배정 카운트다운이 진행 중이라면
		if (TTGameState->RoleAssignmentCountdownTime > 0)
		{
			// 카운트다운 UI를 보여주고 숫자를 업데이트합니다.
			HUDWidgetInstance->ShowPreRoundUI(true);
			HUDWidgetInstance->UpdatePreRoundCountdown(TTGameState->RoleAssignmentCountdownTime);
		}
		// 카운트다운이 끝났다면
		else
		{
			// 카운트다운 UI를 숨기고 메인 게임 타이머를 업데이트합니다.
			HUDWidgetInstance->ShowPreRoundUI(false);
			HUDWidgetInstance->UpdateTimer(TTGameState->RemainingTime);
		}
	}
}

void ATTPlayerController::Server_SendChatMessage_Implementation(const FString& Message)
{
	ATTGameState* TTGameState = GetWorld()->GetGameState<ATTGameState>();
	ATTPlayerState* TTPlayerState = GetPlayerState<ATTPlayerState>();

	if (TTGameState && TTPlayerState)
	{
		// "[닉네임]: 메시지" 형태로 만들어서 GameState에 추가
		FString FormattedMessage = FString::Printf(TEXT("%s: %s"), *TTPlayerState->GetPlayerName(), *Message);
		TTGameState->AddChatMessage(FormattedMessage);
	}
}

void ATTPlayerController::OnChatUpdated_Handle(const TArray<FString>& Messages)
{
	if (HUDWidgetInstance)
	{
		// HUD에게 채팅 내역을 업데이트하라고 전달
		HUDWidgetInstance->UpdateChat(Messages);
	}
}