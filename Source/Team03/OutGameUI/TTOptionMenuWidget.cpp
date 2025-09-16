// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTOptionMenuWidget.h"

#include "OutGameUI/TTUI_PlayerController.h"
#include "OutGameUI/TTGameUserSettings.h"
#include "Components/Button.h"
#include "Components/Slider.h"


void UTTOptionMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 직접 만드신 static Get 함수를 통해 설정 객체를 가져옵니다.
	UTTGameUserSettings* UserSettings = UTTGameUserSettings::GetTTGameUserSettings();
	if (UserSettings)
	{
		// --- 저장된 실제 값을 불러와 UI 값(0~100)으로 변환하여 슬라이더에 표시 ---

		// 사운드: 실제 값(0.0 ~ 1.0) -> UI 값(0 ~ 100)
		SoundVolumeSlider->SetValue(UserSettings->GetSoundVolume() * 100.0f);

		// 마우스 감도: 실제 값(0.5 ~ 1.5) -> UI 값(0 ~ 100) (기본값 1.0이 50이 되도록)
		float UISensitivity = (UserSettings->GetMouseSensitivity() - 0.5f) * 100.0f;
		MouseSensitivitySlider->SetValue(UISensitivity);

		// 밝기(감마): 실제 값(0.5 ~ 5.0) -> UI 값(0 ~ 100) (기본값 2.25가 50 근처가 되도록)
		// (Value - Min) / (Max - Min) 공식을 활용하여 정규화
		float UIGamma = (UserSettings->GetGammaValue() - 0.5f) / 4.5f * 100.0f;
		GammaSlider->SetValue(UIGamma);
	}

	// --- 각 슬라이더 및 버튼의 이벤트에 우리가 만든 함수들을 연결(바인딩)합니다. ---
	SoundVolumeSlider->OnValueChanged.AddDynamic(this, &UTTOptionMenuWidget::OnSoundVolumeChanged);
	GammaSlider->OnValueChanged.AddDynamic(this, &UTTOptionMenuWidget::OnGammaChanged);
	MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &UTTOptionMenuWidget::OnMouseSensitivityChanged);
	BackButton->OnClicked.AddDynamic(this, &UTTOptionMenuWidget::OnBackButtonClicked);
}

// --- UI 값이 변경되면 실제 값으로 변환하여 설정 객체에 임시로 저장합니다. ---
void UTTOptionMenuWidget::OnSoundVolumeChanged(float Value)
{
	UTTGameUserSettings* UserSettings = UTTGameUserSettings::GetTTGameUserSettings();
	// UI 값(0~100) -> 실제 값(0.0~1.0)
	if (UserSettings) UserSettings->SetSoundVolume(Value / 100.0f);
}

void UTTOptionMenuWidget::OnGammaChanged(float Value)
{
	UTTGameUserSettings* UserSettings = UTTGameUserSettings::GetTTGameUserSettings();
	// UI 값(0~100) -> 실제 값(0.5~5.0)
	if (UserSettings) UserSettings->SetGammaValue((Value / 100.0f) * 4.5f + 0.5f);
}

void UTTOptionMenuWidget::OnMouseSensitivityChanged(float Value)
{
	UTTGameUserSettings* UserSettings = UTTGameUserSettings::GetTTGameUserSettings();
	// UI 값(0~100) -> 실제 값(0.5~1.5)
	if (UserSettings) UserSettings->SetMouseSensitivity((Value / 100.0f) * 1.0f + 0.5f);
}

// --- '뒤로가기' 버튼: 변경된 모든 사항을 파일에 최종 저장하고 메인 메뉴로 돌아갑니다. ---
void UTTOptionMenuWidget::OnBackButtonClicked()
{
	UTTGameUserSettings* UserSettings = UTTGameUserSettings::GetTTGameUserSettings();
	if (UserSettings)
	{
		// ApplySettings를 호출하면 SaveConfig() 및 감마 적용까지 모두 처리됩니다.
		UserSettings->ApplySettings(false);
	}

	ATTUI_PlayerController* PlayerController = Cast<ATTUI_PlayerController>(GetOwningPlayer());
	if (PlayerController) PlayerController->ShowMainMenuUI();
}