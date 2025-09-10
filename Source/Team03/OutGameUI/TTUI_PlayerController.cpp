// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTUI_PlayerController.h"

#include "OutGameUI/TTMainMenuWidget.h"
#include "OutGameUI/TTLobbyWidget.h"
#include "Blueprint/UserWidget.h"

void ATTUI_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	// MainMenuWidgetClass 변수에 유효한 블루프린트 클래스가 할당되었는지 확인
	if (MainMenuWidgetClass)
	{
		// 메인 메뉴위젯을 생성히거 화면에 표시
		MainMenuWidgetInstance = CreateWidget<UTTMainMenuWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			// UI와 상호작용할 수 있도록 마우스 커서를 표시하고, 입력 모드를 UI 전용으로 설정
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATTUI_PlayerController::ShowLobbyUI()
{
	// 메인 메뉴 위젯을 화면에서 제거
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
	}

	// 로비 위젯을 생성하고 화면에 표시
	if (LobbyWidgetClass)
	{
		LobbyWidgetInstance = CreateWidget<UTTLobbyWidget>(this, LobbyWidgetClass);
		if (LobbyWidgetInstance)
		{
			LobbyWidgetInstance->AddToViewport();

			// UI와 상호작용할 수 있도록 마우스 커서를 표시하고, 입력 모드를 UI 전용으로 설정
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATTUI_PlayerController::ShowMainMenuUI()
{
	// 로비 위젯을 화면에서 제거
	if (LobbyWidgetInstance)
	{
		LobbyWidgetInstance->RemoveFromParent();
	}
	
	// 메인 메뉴 위젯을 생성하고 화면에 표시
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UTTMainMenuWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
		}
	}
}
