// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTSessionEntryWidget.h"

#include "OutGameUI/TTGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UTTSessionEntryWidget::Setup(const FServerInfo& ServerInfo)
{
	// 전달받은 서버 정보로 UI 텍스트를 설정
	RoomNameText->SetText(FText::FromString(ServerInfo.ServerName));
	FString PlayerCount = FString::Printf(TEXT("%d / %d"), ServerInfo.CurrentPlayers, ServerInfo.MaxPlayers);
	PlayerCountText->SetText(FText::FromString(PlayerCount));

	// 이 방의 인덱스를 저장
	ServerIndex = ServerInfo.SearchResultIndex;
}

void UTTSessionEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &UTTSessionEntryWidget::OnJoinButtonClicked);
	}
}

void UTTSessionEntryWidget::OnJoinButtonClicked()
{
	// GameInstance를 가져옵니다.
	UTTGameInstance* GameInstance = Cast<UTTGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		// 저장된 인덱스를 사용해 GameInstance에 참여를 요청합니다.
		GameInstance->JoinFoundSession(ServerIndex);
	}
}