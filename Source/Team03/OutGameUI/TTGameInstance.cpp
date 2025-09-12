// Fill out your copyright notice in the Description page of Project Settings.


#include "OutGameUI/TTGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

UTTGameInstance::UTTGameInstance()
{
}

// GameInstance가 초기화될 때 호출되는 함수
void UTTGameInstance::Init()
{
	// 온라인 서브시스템(OSS) 초기화
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		//OSS에서 세션 관리 기능(인터페이스)을 가져옴
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem Initialized: %s"), *Subsystem->GetSubsystemName().ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get SessionInterface!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No OnlineSubsystem found! Check DefaultEngine.ini"));
		return;
	}

	// 서버 역할에 따라 동작 분기
	if (IsDedicatedServerInstance())  // 실행하는 인스턴스가 데디케이트 서버인 경우
	{
		// 서버는 시작하자마자 스스로 방(세션)을 생성
		UE_LOG(LogTemp, Warning, TEXT("Running as Dedicated Server - Creating Session..."));
		CreateRoomSession();
	}
}

//UI의 '게임 시작/참여' 버튼 등이 호출하는 메인 함수
void UTTGameInstance::StartGame()
{
	if (SessionInterface.IsValid())
	{ 
		// 클라이언트가 방을 찾기 시작하도록 FindRoomSessions 함수를 호출
		FindRoomSessions();
	}
}

// 서버에서 세션을 생성하는 함수
void UTTGameInstance::CreateRoomSession()
{
	// 세션 생성이 완료되면 OnCreateRoomSessionComplete 함수를 호출하도록 델리게이트를 연결
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegates(this); // 만약을 위해 이전 델리게이트 정리
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTTGameInstance::OnCreateRoomSessionComplete);
		
		// 생성할 세션의 상세 설정
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsDedicated = true;  // 데디케이트 서버 세션임을 명시
		SessionSettings.bIsLANMatch = true;   // LAN 환경에서 테스트하기 위해 true로 설정
		SessionSettings.NumPublicConnections = 2; // 최대 접속 인원
		SessionSettings.bShouldAdvertise = true;  // 다른 클라이언트가 이 세션을 검색할 수 있도록 공개
		SessionSettings.bUsesPresence = true;
		
		// 커스텀 프로퍼티를 사용하여 이 세션을 식별할 수 있는 '꼬리표'
		SessionSettings.Set(FName("GameType"), FString("CopsAndRobbers"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		// 설정한 내용으로 세션 생성을 요청
		SessionInterface->CreateSession(0, FName("My TT Game Session"), SessionSettings);
	}
}

// CreateRoomSession 요청이 완료되었을 때 서버에서 자동으로 호출되는 함수
void UTTGameInstance::OnCreateRoomSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dedicated Session '%s' created successfully."), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create Dedicated Session."));
	}
}

// 클라이언트에서 참여 가능한 세션을 찾는 함수
void UTTGameInstance::FindRoomSessions()
{
	if (SessionInterface.IsValid())
	{
		// 세션 찾기가 완료되면 OnFindRoomSessionsComplete 함수를 호출하도록 델리게이트를 연결
		SessionInterface->ClearOnFindSessionsCompleteDelegates(this);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTTGameInstance::OnFindRoomSessionsComplete);

		// 세션 검색 조건을 설정할 객체를 생성
		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		SessionSearch->MaxSearchResults = 10;  // 최대 10개의 결과만 검색
		SessionSearch->bIsLanQuery = true;   // LAN 환경에서 검색

		// 우리가 생성할 때 설정한 '꼬리표'와 일치하는 세션만 찾도록 조건을 추가
		SessionSearch->QuerySettings.Set(FName("GameType"), FString("CopsAndRobbers"), EOnlineComparisonOp::Equals);
		// 세션 찾기를 요청합니다.
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

// FindRoomSessions 요청이 완료되었을 때 클라이언트에서 자동으로 호출되는 함수
void UTTGameInstance::OnFindRoomSessionsComplete(bool bWasSuccessful)
{
	// 검색에 성공했고, 결과가 1개 이상 있다면
	if (bWasSuccessful && SessionSearch.IsValid() && SessionSearch->SearchResults.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d dedicated servers"), SessionSearch->SearchResults.Num());
		// 테스트를 위해 찾은 첫 번째 세션에 바로 참여
		JoinRoomSession(SessionSearch->SearchResults[0]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No servers found"));
	}
}

// 검색된 세션 결과(SearchResult)를 가지고 실제 참여를 시작하는 함수
void UTTGameInstance::JoinRoomSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (SessionInterface.IsValid())
	{
		// 세션 참여가 완료되면 OnJoinRoomSessionComplete 함수를 호출하도록 델리게이트를 연결
		SessionInterface->ClearOnJoinSessionCompleteDelegates(this);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTTGameInstance::OnJoinRoomSessionComplete);
		// 세션 참여를 요청합니다.
		SessionInterface->JoinSession(0, FName("My TT Game Session"), SearchResult);
	}
}

// JoinRoomSession 요청이 완료되었을 때 클라이언트에서 자동으로 호출되는 함수
void UTTGameInstance::OnJoinRoomSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// 참여에 성공했다면
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		// 참여할 서버의 실제 접속 주소(IP:Port)를 가져옴
		FString ConnectString;
		if (SessionInterface->GetResolvedConnectString(SessionName, ConnectString))
		{
			// 현재 플레이어의 컨트롤러를 가져옴
			APlayerController* PC = GetFirstLocalPlayerController();
			if (PC)
			{
				UE_LOG(LogTemp, Warning, TEXT("Joining server at: %s"), *ConnectString);
				// 해당 주소로 맵 이동을 명령
				PC->ClientTravel(ConnectString, TRAVEL_Absolute);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to join session"));
	}
}

// '게임 종료' 버튼이 호출하는 함수입니다. 세션을 안전하게 정리하고 게임 종료
void UTTGameInstance::CleanupAndExit()
{
	bShouldQuitAfterDestroy = true; // 게임을 종료해야 한다고 기억

	// 현재 유효한 세션이 있는지 확인합니다. "My TT Game Session"은 CreateRoomSession에서 사용한 이름과 일치
	if (SessionInterface.IsValid() && SessionInterface->GetNamedSession(FName("My TT Game Session")) != nullptr)
	{
		// 세션이 있다면, 파괴를 요청
		DestroyRoomSession();
	}
	else
	{
		// 참여 중인 세션이 없다면, 바로 게임을 종료
		APlayerController* PlayerController = GetFirstLocalPlayerController();
		if (PlayerController)
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
		}
	}
}

// '뒤로가기' 버튼 등이 호출하는 함수입니다. 세션을 정리하고 메인로 이동
void UTTGameInstance::DestroyRoomSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegates(this);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UTTGameInstance::OnDestroyRoomSessionComplete);
		SessionInterface->DestroySession(FName("My TT Game Session"));
	}
}

// DestroyRoomSession 요청이 완료되었을 때 호출되는 함수
void UTTGameInstance::OnDestroyRoomSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Destroyed: %s"), *SessionName.ToString());

		// 만약 게임을 종료해야 한다면
		if (bShouldQuitAfterDestroy)
		{
			APlayerController* PlayerController = GetFirstLocalPlayerController();
			if (PlayerController)
			{
				UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
			}
		}
		else // 그렇지 않다면 그냥 방에서 나가는 경우 메인 메뉴로 이동
		{
			APlayerController* PlayerController = GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel("/Game/TT/Maps/OutGameUI/MainMenu_Map", ETravelType::TRAVEL_Absolute);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to destroy session!"));
	}
}