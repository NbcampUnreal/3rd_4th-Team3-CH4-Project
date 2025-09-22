// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "TTGameInstance.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString ServerName;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers = 0;

	// UI에서 Join할 때 사용할 인덱스 (GameInstance 내부 배열 인덱스)
	UPROPERTY(BlueprintReadOnly)
	int32 SearchResultIndex = INDEX_NONE;
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServerListUpdated, const TArray<FServerInfo>&, ServerList);

UCLASS()
class TEAM03_API UTTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTTGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	// 세션을 찾는 함수
	void FindRoomSessions();

	//UI에서 '방 만들기'를 호출하는 함수
	void CreateRoomSession();

	//찾은 세션에 참여하는 함수
	void JoinRoomSession(const FOnlineSessionSearchResult& SearchResult);

	//현재 참여 중인 세션을 파괴
	void DestroyRoomSession();

	//세션을 정리하고 게임을 종료
	void CleanupAndExit();

	void JoinFoundSession(int32 SessionIndex);

	UPROPERTY(BlueprintAssignable)
	FOnServerListUpdated OnServerListUpdated;

	// 모든 레벨에서 접근할 수 있도록 플레이어 이름을 저장할 변수
	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;

	UFUNCTION(BlueprintCallable)
	void LeaveSession();
private:
	
	//세션 생성이 완료되었을 때 엔진이 자동으로 호출할 함수 (델리게이트 콜백)
	void OnCreateRoomSessionComplete(FName SessionName, bool bWasSuccessful);
	
	//세션 찾기 완료 시 호출될 콜백 함수
	void OnFindRoomSessionsComplete(bool bWasSuccessful);
	
	//세션 참여 완료 시 호출될 콜백 함수
	void OnJoinRoomSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//세션 파괴가 완료되었을 때 호출될 콜백 함수
	void OnDestroyRoomSessionComplete(FName SessionName, bool bWasSuccessful);

	//세션 파괴 후 게임을 종료해야 하는지 여부를 기억하는 변수
	bool bShouldQuitAfterDestroy = false;

	//온라인 세션 인터페이스를 가리키는 포인터. 세션 관련 작업을 처리하는 핵심 도구
	IOnlineSessionPtr SessionInterface;

	//세션 검색 결과를 저장할 포인터
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// '빠른 참가' (자동 참여)를 시도 중인지 여부를 기억하는 변수
	bool bIsTryingToAutoJoin = false;

	IOnlineSessionPtr GetSessionInterface();

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

};

