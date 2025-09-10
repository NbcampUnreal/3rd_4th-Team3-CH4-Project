// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TTUI_PlayerController.generated.h"

class UTTMainMenuWidget;
class UTTLobbyWidget;
/**
 * 
 */
UCLASS()
class TEAM03_API ATTUI_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	//메인 메뉴 UI를 숨기고 로비 UI를 표시
	void ShowLobbyUI();

	//로비 UI를 숨기고 메인 메뉴 UI를 다시 표시
	void ShowMainMenuUI();
	
protected:
	virtual void BeginPlay() override;

private:
	// MainMenuUI 위젯 클래스 변수
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTTMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY()
	UTTMainMenuWidget* MainMenuWidgetInstance;

	// LobbyUI 위젯 클래스 변수
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTTLobbyWidget> LobbyWidgetClass;
	UPROPERTY()
	UTTLobbyWidget* LobbyWidgetInstance;
};
