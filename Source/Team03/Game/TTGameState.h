#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TTGameState.generated.h"

// 델리게이트 선언: 채팅 메시지 배열이 업데이트될 때 UI에게 알려줄 신호
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChatUpdatedDelegate, const TArray<FString>& /* ChatMessages */);

UCLASS()
class TEAM03_API ATTGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ATTGameState();

	// 변수 복제를 위한 필수 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// GameMode가 호출할 함수. 서버에서 새 메시지를 배열에 추가
	void AddChatMessage(const FString& Message);

	// UI가 바인딩할 델리게이트
	FOnChatUpdatedDelegate OnChatUpdated;

	// 게임의 남은 시간 변수 (10분)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category = "Game")
	int32 RemainingTime;

	// 역할 배정 카운트다운 시간 변수 (10초)
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Replicated, Category = "Game")
	int32 RoleAssignmentCountdownTime;

private:
	// 채팅 메시지를 저장할 배열
	UPROPERTY(ReplicatedUsing = OnRep_ChatMessages)
	TArray<FString> ChatMessages;

	// ChatMessages 배열이 복제될 때 클라이언트에서 호출될 함수
	UFUNCTION()
	void OnRep_ChatMessages();
};
