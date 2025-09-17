// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTOptionMenuWidget.generated.h"

class UButton;
class USlider;
/**
 * 
 */
UCLASS()
class TEAM03_API UTTOptionMenuWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta=(BindWidget))
	USlider* SoundVolumeSlider;
	UPROPERTY(meta=(BindWidget))
	USlider* GammaSlider;
	UPROPERTY(meta=(BindWidget))
	USlider* MouseSensitivitySlider;
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	// 슬라이더 값 변경 시 호출될 함수들
	UFUNCTION()
	void OnSoundVolumeChanged(float Value);
	UFUNCTION()
	void OnGammaChanged(float Value);
	UFUNCTION()
	void OnMouseSensitivityChanged(float Value);
	
	// 저장 누르면 호출될 함수
	UFUNCTION()
	void OnSaveButtonClicked();
	// 뒤로가기 누르면 호출될 함수
	UFUNCTION()
	void OnBackButtonClicked();
};
