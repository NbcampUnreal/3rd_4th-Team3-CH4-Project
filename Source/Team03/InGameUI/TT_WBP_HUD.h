#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TT_WBP_HUD.generated.h"

UCLASS()
class TEAM03_API UTT_WBP_HUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    //위젯이 처음 생성될 때 호출되는 함수
    virtual void NativeConstruct() override;

    //UPROPERTY(meta = (BindWidget))
    //TObjectPtr<UProgressBar> HPBar;
};

