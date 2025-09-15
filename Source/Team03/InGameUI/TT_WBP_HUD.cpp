#include "InGameUI/TT_WBP_HUD.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"      
#include "Components/EditableTextBox.h" 
#include "Character/TTPlayerController.h" 

void UTT_WBP_HUD::NativeConstruct()
{
    Super::NativeConstruct();

    //이 곳에 위젯이 처음 생성될 때 실행할 초기화 코드를 작성
	ShowPreRoundUI(false);

	// 채팅 입력창의 OnTextCommitted 이벤트에 함수를 바인딩
	if (ChatInputBox)
	{
		ChatInputBox->OnTextCommitted.AddDynamic(this, &UTT_WBP_HUD::OnChatTextCommitted);
	}
}

void UTT_WBP_HUD::UpdateTimer(int32 RemainingTime)
{
	if (TimerText)
	{
		// 남은 시간을 분:초 형태의 문자열로 변환
		int32 Minutes = RemainingTime / 60;
		int32 Seconds = RemainingTime % 60;
		FString TimerString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);

		// TextBlock의 텍스트를 변경
		TimerText->SetText(FText::FromString(TimerString));
	}
}

void UTT_WBP_HUD::UpdatePreRoundCountdown(int32 CountdownTime)
{
	if (PreRoundCountdownText)
	{
		FString CountdownString = FString::Printf(TEXT("포지션 배정중...%d"), CountdownTime);
		PreRoundCountdownText->SetText(FText::FromString(CountdownString));
	}
}

void UTT_WBP_HUD::ShowPreRoundUI(bool bShow)
{
	if (PreRoundCountdownText)
	{
		PreRoundCountdownText->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	// 카운트다운이 보일 때는 메인 타이머는 숨깁니다.
	if (TimerText)
	{
		TimerText->SetVisibility(bShow ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void UTT_WBP_HUD::UpdateChat(const TArray<FString>& Messages)
{
	if (ChatScrollBox)
	{
		// 채팅창을 새로 그리기 전에 기존 내용을 모두 지웁니다.
		ChatScrollBox->ClearChildren();

		// 모든 메시지를 순회하며 TextBlock 위젯으로 만들어 추가
		for (const FString& Msg : Messages)
		{
			UTextBlock* NewMessage = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
			if (NewMessage)
			{
				NewMessage->SetText(FText::FromString(Msg));
				// 여기에 폰트, 색상 등 스타일을 설정
				ChatScrollBox->AddChild(NewMessage);
			}
		}

		// 새 메시지가 오면 항상 스크롤을 맨 아래로
		ChatScrollBox->ScrollToEnd();
	}
}

void UTT_WBP_HUD::OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// Enter 키를 눌렀을 때만 작동하고, 입력 내용이 비어있지 않은지 확인
	if (CommitMethod == ETextCommit::OnEnter && !Text.IsEmpty())
	{
		ATTPlayerController* PC = GetOwningPlayer<ATTPlayerController>();
		if (PC)
		{
			// PlayerController에게 서버로 메시지를 보내달라고 요청
			PC->Server_SendChatMessage(Text.ToString());

			// 메시지를 보낸 후 입력창을 비웁니다.
			ChatInputBox->SetText(FText::GetEmpty());
		}
	}
}