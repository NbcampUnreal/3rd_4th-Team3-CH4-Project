// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTUI_PlayerController.h"

#include "OutGameUI/TTMainMenuWidget.h"
#include "Blueprint/UserWidget.h"

void ATTUI_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	// MainMenuWidgetClass 변수에 유효한 블루프린트 클래스가 할당되었는지 확인
	if (MainMenuWidgetClass)
	{
		// 위젯을 생성
		MainMenuWidgetInstance = CreateWidget<UTTMainMenuWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			// 생성된 위젯을 화면에 추가
			MainMenuWidgetInstance->AddToViewport();

			// UI와 상호작용할 수 있도록 마우스 커서를 표시하고, 입력 모드를 UI 전용으로 설정
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}
