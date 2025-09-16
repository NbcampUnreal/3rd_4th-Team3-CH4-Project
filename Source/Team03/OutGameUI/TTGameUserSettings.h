// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "TTGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class TEAM03_API UTTGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UTTGameUserSettings();


    // 정적 Getter (블루프린트에서도 바로 불러오기 가능)
    UFUNCTION(BlueprintPure, Category = "Settings")
    static UTTGameUserSettings* GetTTGameUserSettings();

    // 사운드 볼륨
    UFUNCTION(BlueprintPure, Category = "Settings|Sound")
    float GetSoundVolume() const;
    UFUNCTION(BlueprintCallable, Category = "Settings|Sound")
    void SetSoundVolume(float Value);

    // 마우스 감도
    UFUNCTION(BlueprintPure, Category = "Settings|Mouse")
    float GetMouseSensitivity() const;
    UFUNCTION(BlueprintCallable, Category = "Settings|Mouse")
    void SetMouseSensitivity(float Value);

    // 감마 
    UFUNCTION(BlueprintPure, Category = "Settings|Graphics")
    float GetGammaValue() const;
    UFUNCTION(BlueprintCallable, Category = "Settings|Graphics")
    void SetGammaValue(float Value);

    // 저장 적용 오버라이드
    virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

protected:
	// UPROPERTY(Config)는 이 변수들이 .ini 파일에 자동으로 저장
    UPROPERTY(Config)
    float SoundVolume;

    UPROPERTY(Config)
    float MouseSensitivity;

    UPROPERTY(Config)
    float GammaValue;   // 새로 추가
};
