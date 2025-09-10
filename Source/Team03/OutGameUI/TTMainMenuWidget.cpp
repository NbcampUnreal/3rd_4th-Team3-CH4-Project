// Fill out your copyright notice in the Description page of Project Settings.


#include "TTMainMenuWidget.h"

#include "OutGameUI/TTUI_PlayerController.h" 
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

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
    UE_LOG(LogTemp, Warning, TEXT("Option Button Clicked!"));
}

void UTTMainMenuWidget::OnExitButtonClicked()
{
    APlayerController* PlayerController = GetOwningPlayer();
    
    // QuitGame 함수를 호출하여 게임을 종료
    UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
