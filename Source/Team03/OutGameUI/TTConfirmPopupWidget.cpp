// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTConfirmPopupWidget.h"

#include "OutGameUI/TTMainMenuWidget.h"
#include "OutGameUI/TTGameInstance.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UTTConfirmPopupWidget::NativeConstruct()
{
    if (ConfirmButton)
    {
        ConfirmButton->OnClicked.AddDynamic(this, &UTTConfirmPopupWidget::OnConfirmButtonClicked);
    }

    if (CancelButton)
    {
        CancelButton->OnClicked.AddDynamic(this, &UTTConfirmPopupWidget::OnCancelButtonClicked);
    }
}

void UTTConfirmPopupWidget::OnConfirmButtonClicked()
{
    UTTGameInstance* GameInstance = Cast<UTTGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (GameInstance)
    {
        // GameInstance에게 정리 및 종료를 요청합니다.
        GameInstance->CleanupAndExit();
    }
}

void UTTConfirmPopupWidget::OnCancelButtonClicked()
{
    // 팝업 닫기
    RemoveFromParent();

    if (ExitButtonRef)
    {
        ExitButtonRef->SetIsEnabled(true);
    }
}