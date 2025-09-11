// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TT_Team.h"
#include "TTPlayerState.generated.h"

UCLASS()
class TEAM03_API ATTPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATTPlayerState();

	// ReplicatedUsing은 변수가 클라이언트에서 바뀔 때마다 OnRep_Team 함수를 자동 호출
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, ReplicatedUsing = OnRep_Team, Category = "Team")
	ETeam Team;

	// 서버의 데이터를 클라이언트로 복제하기 위한 필수 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 팀이 변경되었을 때 UI를 업데이트 하도록 PlayerController에게 알려주는 델리게이트
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamChangedDelegate, ETeam);
	FOnTeamChangedDelegate OnTeamChanged;

protected:
	UFUNCTION()
	void OnRep_Team(); // Team 변수가 복제될 때 호출될 함수
};
