// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTLobbyWidget.h"

#include "OutGameUI/TTUI_PlayerController.h"
#include "Components/Button.h"

void UTTLobbyWidget::NativeConstruct()
{
	if (RefreshButton)
	{
		RefreshButton->OnClicked.AddDynamic(this, &UTTLobbyWidget::OnRefreshButtonClicked);
	}

	if (CreateRoomButton)
	{
		CreateRoomButton->OnClicked.AddDynamic(this, &UTTLobbyWidget::OnCreateRoomButtonClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UTTLobbyWidget::OnBackButtonClicked);
	}
}

// 지금은 로그만 출력하도록 설정 각 버튼에 맞는 기능 구현 필요
void UTTLobbyWidget::OnRefreshButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Refresh Button Clicked!"));
}

void UTTLobbyWidget::OnCreateRoomButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Create Room Button Clicked!"));
}

void UTTLobbyWidget::OnBackButtonClicked()
{
	ATTUI_PlayerController* UI_PlayerController = Cast<ATTUI_PlayerController>(GetOwningPlayer());
	if (UI_PlayerController)
	{
		// 컨트롤러에게 메인 메뉴 UI를 다시 보여달라고 요청
		UI_PlayerController->ShowMainMenuUI();
	}
}