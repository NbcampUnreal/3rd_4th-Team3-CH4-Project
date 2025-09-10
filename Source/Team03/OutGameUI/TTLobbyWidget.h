// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTLobbyWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class TEAM03_API UTTLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	// 블루프린트의 버튼 위젯과 연결될 변수들
	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateRoomButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	//각 버튼이 클릭 되었을 때 호출되는 함수
	UFUNCTION()
	void OnRefreshButtonClicked();

	UFUNCTION()
	void OnCreateRoomButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();
};
