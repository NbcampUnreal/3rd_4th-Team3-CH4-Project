#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TT_WBP_HUD.generated.h"

class UTextBlock;

UCLASS()
class TEAM03_API UTT_WBP_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// PlayerController가 호출할 함수 -> 남은 시간
	void UpdateTimer(int32 RemainingTime);


protected:
    //위젯이 처음 생성될 때 호출되는 함수
    virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;
};

