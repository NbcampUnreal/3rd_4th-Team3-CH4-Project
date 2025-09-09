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
    UPROPERTY(meta = (BindWidget))
    UButton* StartButton;

    UPROPERTY(meta = (BindWidget))
    UButton* OptionButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ExitButton;
    
    //각 버튼이 클릭 되었을 때 호출되는 함수
    UFUNCTION()
    void OnStartButtonClicked();

    UFUNCTION()
    void OnOptionButtonClicked();

    UFUNCTION()
    void OnExitButtonClicked();
};
