// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTGameUserSettings.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

UTTGameUserSettings::UTTGameUserSettings()
{
	SoundVolume = 0.5f;
	MouseSensitivity = 1.0f;
	GammaValue = 2.25f;
}

// static 함수 구현
UTTGameUserSettings* UTTGameUserSettings::GetTTGameUserSettings()
{
	return Cast<UTTGameUserSettings>(GEngine->GetGameUserSettings());
}

// --- Getter / Setter 구현 ---
// 사운드 볼륨
float UTTGameUserSettings::GetSoundVolume() const
{
    return SoundVolume;
}

void UTTGameUserSettings::SetSoundVolume(float Value)
{
    SoundVolume = FMath::Clamp(Value, 0.0f, 1.0f);
}

// 마우스 감도
float UTTGameUserSettings::GetMouseSensitivity() const
{
    return MouseSensitivity;
}

void UTTGameUserSettings::SetMouseSensitivity(float Value)
{
    MouseSensitivity = FMath::Max(0.01f, Value); // 0으로 나눠지는 문제 방지
}

// 감마
float UTTGameUserSettings::GetGammaValue() const
{
    return GammaValue;
}

void UTTGameUserSettings::SetGammaValue(float Value)
{
    GammaValue = FMath::Clamp(Value, 0.5f, 5.0f); // 감마는 일반적으로 0.5 ~ 5.0 범위
}

// ini 저장 반영
void UTTGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
    Super::ApplySettings(bCheckForCommandLineOverrides);

    // SaveConfig()는 Super::ApplySettings()가 내부적으로 호출해주므로 중복 호출할 필요가 없을 수 있습니다.
    // 하지만 명시적으로 호출하는 것이 더 안전할 수 있습니다.
    SaveConfig();

    // 현재 월드에 감마 적용 (콘솔 명령 방식)
    if (GEngine)
    {
        // GEngine이 관리하는 모든 월드 컨텍스트를 순회합니다.
        for (const FWorldContext& Context : GEngine->GetWorldContexts())
        {
            // PIE(에디터에서 플레이) 또는 Game(빌드된 게임) 타입의 월드를 찾습니다.
            if (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE)
            {
                UWorld* World = Context.World();
                if (World)
                {
                    // GetGammaValue()로 우리가 만든 변수 값을 가져옵니다.
                    const FString Cmd = FString::Printf(TEXT("gamma %f"), GetGammaValue());
                    UKismetSystemLibrary::ExecuteConsoleCommand(World, Cmd);

                    // 여러 월드가 있을 수 있으므로, 하나만 적용하고 루프를 빠져나옵니다.
                    break;
                }
            }
        }
    }
}