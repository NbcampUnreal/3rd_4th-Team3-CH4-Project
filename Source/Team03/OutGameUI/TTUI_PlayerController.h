// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TTUI_PlayerController.generated.h"

class UTTMainMenuWidget;
/**
 * 
 */
UCLASS()
class TEAM03_API ATTUI_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	// UI 위젯 클래스 변수
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTTMainMenuWidget> MainMenuWidgetClass;
	UPROPERTY()
	UTTMainMenuWidget* MainMenuWidgetInstance;
};
