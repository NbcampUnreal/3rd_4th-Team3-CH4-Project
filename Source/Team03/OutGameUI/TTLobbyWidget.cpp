// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTLobbyWidget.h"

#include "OutGameUI/TTUI_PlayerController.h"
#include "OutGameUI/TTGameInstance.h"
#include "OutGameUI/TTSessionEntryWidget.h" 
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

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

	UTTGameInstance* GameInstance = Cast<UTTGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		// GameInstance의 OnServerListUpdated 신호가 오면, UpdateSessionList 함수를 실행하도록 구독합니다.
		GameInstance->OnServerListUpdated.AddDynamic(this, &UTTLobbyWidget::UpdateSessionList);
	}
}

// 각 버튼에 맞는 기능 구현
void UTTLobbyWidget::OnRefreshButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Refresh Button Clicked! Finding sessions..."));

	UTTGameInstance* GameInstance = Cast<UTTGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->FindRoomSessions();
	}
}

void UTTLobbyWidget::OnCreateRoomButtonClicked()
{
	// 이 부분이 GameInstance의 기능을 호출하는 코드
	UTTGameInstance* GameInstance = Cast<UTTGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->StartGame();
	}
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

void UTTLobbyWidget::UpdateSessionList(const TArray<FServerInfo>& ServerList)
{
	// 스크롤 박스가 유효하지 않으면 함수를 종료
	if (!ServerList_ScrollBox) return;

	// 목록을 업데이트하기 전에 기존 목록을 모두 삭제
	ServerList_ScrollBox->ClearChildren();

	// 찾은 세션 결과 각각에 대해 '방 한 줄' UI를 생성
	for (const FServerInfo& Info : ServerList)
	{
		// 블루프린트에서 지정한 SessionEntryClass를 기반으로 위젯을 동적으로 생성
		UTTSessionEntryWidget* SessionEntry = CreateWidget<UTTSessionEntryWidget>(this, SessionEntryClass);
		if (SessionEntry)
		{
			SessionEntry->Setup(Info); // 위젯에 세션 정보 설정
			ServerList_ScrollBox->AddChild(SessionEntry); // 스크롤 박스에 자식으로 추가
		}
	}
}
