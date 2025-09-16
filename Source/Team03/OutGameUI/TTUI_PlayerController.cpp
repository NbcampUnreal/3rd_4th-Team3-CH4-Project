// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTUI_PlayerController.h"

#include "OutGameUI/TTMainMenuWidget.h"
#include "OutGameUI/TTLobbyWidget.h"
#include "OutGameUI/TTOptionMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ATTUI_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FString MapName = GetWorld()->GetMapName();

	if (MapName.Contains(TEXT("MainMenu_Map")))
	{
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
	else
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
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
	// 옵션 메뉴 위젯이 화면에 있다면 제거
	if (OptionsMenuWidgetInstance)
	{
		OptionsMenuWidgetInstance->RemoveFromParent();
		OptionsMenuWidgetInstance = nullptr; // 포인터를 정리
	}

	// 로비 위젯을 화면에서 제거
	if (LobbyWidgetInstance)
	{
		LobbyWidgetInstance->RemoveFromParent();
		LobbyWidgetInstance = nullptr;
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

void ATTUI_PlayerController::ShowOptionsMenu()
{
	// 기존 메인 메뉴는 숨깁니다.
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
	}

	// 옵션 메뉴 위젯을 생성하고 화면에 표시합니다.
	if (OptionsMenuWidgetClass)
	{
		OptionsMenuWidgetInstance = CreateWidget<UTTOptionMenuWidget>(this, OptionsMenuWidgetClass);
		if (OptionsMenuWidgetInstance)
		{
			OptionsMenuWidgetInstance->AddToViewport();
		}
	}
}

void ATTUI_PlayerController::HideOptionsMenu()
{
	// 메인 메뉴를 다시 표시
	ShowMainMenuUI();
}

