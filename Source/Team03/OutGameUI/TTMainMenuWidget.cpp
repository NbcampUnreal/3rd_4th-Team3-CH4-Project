// Fill out your copyright notice in the Description page of Project Settings.


#include "TTMainMenuWidget.h"

#include "OutGameUI/TTConfirmPopupWidget.h"
#include "OutGameUI/TTUI_PlayerController.h" 
#include "OutGameUI/TTGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
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

    if (Player)
    {
        Player->OnTextChanged.AddDynamic(this, &UTTMainMenuWidget::OnPlayerNameTextChanged);
    }
}


void UTTMainMenuWidget::OnStartButtonClicked()
{
    UTTGameInstance* GameInstance = Cast<UTTGameInstance>(GetGameInstance());
    if (GameInstance && Player)
    {
        // 입력창의 텍스트를 가져와서 GameInstance에 저장
        GameInstance->PlayerName = Player->GetText().ToString();
    }

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

// TextBox 글자 수 제한
void UTTMainMenuWidget::OnPlayerNameTextChanged(const FText& Text)
{
    const int32 MaxLength = 8; // 글자 수
    FString CurrentText = Text.ToString();
    if (CurrentText.Len() > MaxLength && Player)
    {
        // 최대 길이를 초과하면 잘라서 다시 설정
        FString TrimmedText = CurrentText.Left(MaxLength);
        Player->SetText(FText::FromString(TrimmedText));
    }
}