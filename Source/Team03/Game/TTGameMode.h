#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TTGameMode.generated.h"

// .h 파일에서는 class 키워드로 전방 선언하여 헤더 파일을 가볍게 유지합니다.
class ATTGameState;

UCLASS()
class TEAM03_API ATTGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATTGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	// 경기가 시작될 때 호출되는 함수
	virtual void HandleMatchHasStarted() override;

protected:
	// 경찰 캐릭터로 사용할 블루프린트 클래스를 에디터에서 지정할 수 있도록 변수를 노출시킵니다.
	UPROPERTY(EditDefaultsOnly, Category = "Classes")
	TSubclassOf<APawn> PolicePawnClass;
};