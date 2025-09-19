// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTMainMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class TEAM03_API UTTMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
    // 블루프린트의 버튼 위젯과 연결될 변수들
    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

    UPROPERTY(meta = (BindWidget))
    UButton* OptionButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ExitButton;

    // 블루프린트의 EditableTextBox와 연결될 변수
    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* Player;

    UPROPERTY(EditDefaultsOnly, Category = "Popup")
    TSubclassOf<class UTTConfirmPopupWidget> ConfirmPopupClass;

    UPROPERTY()
    UButton* ExitButtonRef;

    void SetExitButton(UButton* InButton) { ExitButtonRef = InButton; }

    //각 버튼이 클릭 되었을 때 호출되는 함수
    UFUNCTION()
    void OnStartButtonClicked();

    UFUNCTION()
    void OnOptionButtonClicked();

    UFUNCTION()
    void OnExitButtonClicked();

    UFUNCTION()
    void OnPlayerNameTextChanged(const FText& Text);
};
