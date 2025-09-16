// Fill out your copyright notice in the Description page of Project Settings.


#include "TTMainMenuWidget.h"

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

    UTTGameInstance* GameInstance = Cast<UTTGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        // GameInstance에게 정리 및 종료를 요청합니다.
        GameInstance->CleanupAndExit();
    }
}
