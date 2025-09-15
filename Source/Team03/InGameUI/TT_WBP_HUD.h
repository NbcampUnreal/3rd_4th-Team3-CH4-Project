#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TT_WBP_HUD.generated.h"

class UTextBlock;
class UScrollBox;
class UEditableTextBox;

UCLASS()
class TEAM03_API UTT_WBP_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// PlayerController가 호출할 함수 -> 남은 시간
	void UpdateTimer(int32 RemainingTime);

	void UpdatePreRoundCountdown(int32 CountdownTime);

	void ShowPreRoundUI(bool bShow);

	// PlayerController가 호출할 채팅 업데이트 함수
	void UpdateChat(const TArray<FString>& Messages);

protected:
    //위젯이 처음 생성될 때 호출되는 함수
    virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PreRoundCountdownText;

	// 채팅 UI 위젯들과 연결될 변수들
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ChatScrollBox;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ChatInputBox;

private:
	// 채팅 입력창에서 Enter를 누르면 호출될 함수
	UFUNCTION()
	void OnChatTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};

