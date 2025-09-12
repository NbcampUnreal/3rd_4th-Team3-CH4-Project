// TTPlayerController.cpp

#include "Character/TTPlayerController.h"

// 게임 시작 마우스 커서 숨기기 및 게임 전용 입력 모드 설정
void ATTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}
// 서버 로그에 어떤 폰을 소유했는지 출력
void ATTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("Possessed Pawn: %s"), *GetNameSafe(InPawn));
}
