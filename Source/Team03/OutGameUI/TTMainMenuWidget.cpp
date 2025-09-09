// Fill out your copyright notice in the Description page of Project Settings.


#include "TTMainMenuWidget.h"
#include "Components/Button.h"

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
    UE_LOG(LogTemp, Warning, TEXT("Start Button Clicked!"));
}

void UTTMainMenuWidget::OnOptionButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Option Button Clicked!"));
}

void UTTMainMenuWidget::OnExitButtonClicked()
{
    UE_LOG(LogTemp, Warning, TEXT("Exit Button Clicked!"));
}
