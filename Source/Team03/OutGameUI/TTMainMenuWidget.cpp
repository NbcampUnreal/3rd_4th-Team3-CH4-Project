// Fill out your copyright notice in the Description page of Project Settings.


#include "TTMainMenuWidget.h"

#include "OutGameUI/TTConfirmPopupWidget.h"
#include "OutGameUI/TTUI_PlayerController.h" 
#include "OutGameUI/TTGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UTTMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UTTMainMenuWidget::OnStartButtonClicked);
    }

    if (OptionButton)
    {
        OptionButton->OnClicked.AddDynamic(this, &UTTMainMenuWidget::OnOptionButtonClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UTTMainMenuWidget::OnExitButtonClicked);
    }
}


// 지금은 로그만 출력하도록 설정 각 버튼에 맞는 기능 구현 필요
void UTTMainMenuWidget::OnStartButtonClicked()
{
    ATTUI_PlayerController* UI_PlayerController = Cast<ATTUI_PlayerController>(GetOwningPlayer());
    if (UI_PlayerController)
    {
        // 컨트롤러에게 로비 UI를 보여달라고 요청
        UI_PlayerController->ShowLobbyUI();
    }
}

void UTTMainMenuWidget::OnOptionButtonClicked()
{
    ATTUI_PlayerController* PlayerController = Cast<ATTUI_PlayerController>(GetOwningPlayer());
    if (PlayerController)
    {
        // 컨트롤러에게 옵션 메뉴를 보여달라고 요청
        PlayerController->ShowOptionsMenu();
    }
}

void UTTMainMenuWidget::OnExitButtonClicked()
{
    if (ConfirmPopupClass)
    {
        UTTConfirmPopupWidget* Popup = CreateWidget<UTTConfirmPopupWidget>(this, ConfirmPopupClass);
        if (Popup)
        {
            Popup->SetExitButton(ExitButton);
            Popup->AddToViewport(); // 팝업창을 화면에 띄웁니다.
        }
    }
    if (ExitButton)
    {
        ExitButton->SetIsEnabled(false);
    }

}