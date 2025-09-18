// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TTConfirmPopupWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEAM03_API UTTConfirmPopupWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

    void SetExitButton(UButton* InButton)
    {
        ExitButtonRef = InButton;
    }

protected:
    // 바인딩할 버튼 (UMG에서 이름 맞춰야 함)
    UPROPERTY(meta = (BindWidget))
    class UButton* ConfirmButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CancelButton;

    UPROPERTY()
    UButton* ExitButtonRef;

    UFUNCTION()
    void OnConfirmButtonClicked();
    UFUNCTION()
    void OnCancelButtonClicked();

};