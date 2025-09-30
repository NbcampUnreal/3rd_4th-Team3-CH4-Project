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
		// 자동 참여를 시도한다고 표시
		bIsTryingToAutoJoin = true;
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
		SessionSettings.bIsLANMatch = false;  
		SessionSettings.NumPublicConnections = 3;   // 최대 인원	
		SessionSettings.bShouldAdvertise = true;  // 다른 클라이언트가 이 세션을 검색할 수 있도록 공개
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinInProgress = true; 

		// 커스텀 프로퍼티를 사용하여 이 세션을 식별할 수 있는 '꼬리표'
		SessionSettings.Set(FName("GameType"), FString("CopsAndRobbers neoman omyeon gogo"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
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
	// '빠른 참가'가 아닐 경우에만 의도를 false로 설정 StartGame()을 통해 호출될 수도 있기 때문
	if (!bIsTryingToAutoJoin)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finding sessions for refresh..."));
	}
	if (SessionInterface.IsValid())
	{
		// 세션 찾기가 완료되면 OnFindRoomSessionsComplete 함수를 호출하도록 델리게이트를 연결
		SessionInterface->ClearOnFindSessionsCompleteDelegates(this);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTTGameInstance::OnFindRoomSessionsComplete);

		// 세션 검색 조건을 설정할 객체를 생성
		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		SessionSearch->MaxSearchResults = 10;  // 최대 10개의 결과만 검색
		SessionSearch->bIsLanQuery = false;

		SessionSearch->QuerySettings.Set(FName("GameType"), FString("CopsAndRobbers neoman omyeon gogo"), EOnlineComparisonOp::Equals);
		// 세션 찾기를 요청합니다.
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

// FindRoomSessions 요청이 완료되었을 때 클라이언트에서 자동으로 호출되는 함수
void UTTGameInstance::OnFindRoomSessionsComplete(bool bWasSuccessful)
{
	// 만약 '빠른 참가'를 시도 중이었다면
	if (bIsTryingToAutoJoin)
	{
		// 시도가 끝났으므로, 플래그를 다시 false로 리셋합니다.
		bIsTryingToAutoJoin = false;

		// 방을 찾았다면
		if (bWasSuccessful && SessionSearch.IsValid() && SessionSearch->SearchResults.Num() > 0)
		{
			// 첫 번째 방에 바로 참여합니다.
			JoinRoomSession(SessionSearch->SearchResults[0]);
		}
		else // 참여할 방이 없다면
		{
			// 새로운 방을 만듭니다.
			UE_LOG(LogTemp, Warning, TEXT("Auto-join failed: No sessions found. Please refresh the server list."));
		}
	}
	else // 단순 '새로고침'이었다면
	{
		if (bWasSuccessful && SessionSearch.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("FindSessions successful. Found %d sessions."), SessionSearch->SearchResults.Num());
			// FServerInfo를 담을 빈 배열을 생성
			TArray<FServerInfo> ServerInfoList;

			for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
			{
				const FOnlineSessionSearchResult& Result = SessionSearch->SearchResults[i];

				FServerInfo Info;
				FString RoomName;
				if (!Result.Session.SessionSettings.Get(FName("GameType"), RoomName))// 방 이름을 세션 설정에서 가져오기 시도
				{
					// 값이 없으면 호스트 이름이나 SessionId로 대체
					RoomName = Result.Session.OwningUserName;
				}

				Info.ServerName = RoomName;
				Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;

				// 세션에 저장된 CurrentPlayers 가져오기
				int32 CurrentPlayers = 0;
				Result.Session.SessionSettings.Get(FName("CurrentPlayers"), CurrentPlayers);
				Info.CurrentPlayers = CurrentPlayers;

				Info.SearchResultIndex = i;

				ServerInfoList.Add(Info);
			}

			// 깔끔하게 정리된 FServerInfo 배열을 UI에 방송
			OnServerListUpdated.Broadcast(ServerInfoList);
		}
		else
		{
			OnServerListUpdated.Broadcast(TArray<FServerInfo>());
		}
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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to destroy session!"));
	}
}

// UI에서 선택한 세션에 참여하는 함수
void UTTGameInstance::JoinFoundSession(int32 SessionIndex)
{
	// SessionIndex가 유효한 범위에 있고, 검색 결과가 존재하는지 확인합니다.
	if (SessionSearch.IsValid() && SessionSearch->SearchResults.IsValidIndex(SessionIndex))
	{
		// 해당 인덱스의 세션에 참여합니다.
		JoinRoomSession(SessionSearch->SearchResults[SessionIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to join an invalid session index."));
	}
}

IOnlineSessionPtr UTTGameInstance::GetSessionInterface()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		return Subsystem->GetSessionInterface();
	}
	return nullptr;
}

void UTTGameInstance::LeaveSession()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr LocalSessionInterface = Subsystem->GetSessionInterface();
		if (LocalSessionInterface.IsValid())
		{
			// 델리게이트가 아직 바인딩되지 않았다면 바인딩
			OnDestroySessionCompleteDelegateHandle = LocalSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UTTGameInstance::OnDestroySessionComplete));

			if (!LocalSessionInterface->DestroySession("My TT Game Session"))
			{
				// 파괴 요청 자체가 실패한 경우 (예: 세션 인터페이스가 바쁨)
				// 핸들을 다시 클리어하고 UI에 실패를 알리는 등의 처리
				LocalSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			}
		}
	}
}

void UTTGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr LocalSessionInterface = Subsystem->GetSessionInterface();
		if (LocalSessionInterface.IsValid())
		{
			// 사용했던 델리게이트 핸들 정리
			LocalSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		}
	}
	// 세션 파괴에 성공했다면, 
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		if (World)
		{
			for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{

				APlayerController* PC = Iterator->Get();
				if (PC)
				{
					PC->ClientTravel("/Game/TT/Maps/OutGameUI/MainMenu_Map", ETravelType::TRAVEL_Absolute);
				}
			}
		}
	}
}