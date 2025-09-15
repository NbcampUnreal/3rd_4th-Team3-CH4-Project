#include "InGameUI/TT_WBP_HUD.h"
#include "Components/TextBlock.h"

void UTT_WBP_HUD::NativeConstruct()
{
    Super::NativeConstruct();

    //이 곳에 위젯이 처음 생성될 때 실행할 초기화 코드를 작성
    //체력 바를 100%로 채우기.. 기타 등등
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