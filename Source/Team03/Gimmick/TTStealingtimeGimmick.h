// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/TTGimmickBase.h"
#include "TTStealingtimeGimmick.generated.h"

class ATTCharacterThief;

UCLASS()
class TEAM03_API ATTStealingtimeGimmick : public ATTGimmickBase
{
	GENERATED_BODY()
	
public:
	ATTStealingtimeGimmick();

protected:
	// **ATTGimmickBase의 가상 함수 오버라이딩**
	virtual void OnBeginOverlap(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Sweep) override;

	// **Overlap End 이벤트 처리를 위한 함수 선언**
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex);

	// **10초 오버랩 완료 시 실행될 함수**
	void StealingCompleted();

	// **실제 이펙트(시간 절반 감소)를 구현할 함수 오버라이딩**
	virtual void ExecuteEffect_Implementation(AActor* Activator) override;

private:
	// **현재 오버랩 중인 도둑을 저장 (서버 전용)**
	UPROPERTY()
	ATTCharacterThief* OverlappingThief;

	// **훔치기 타이머 핸들**
	FTimerHandle StealingTimerHandle;

	// **훔치기 완료에 필요한 시간 (에디터에서 10.0초로 설정 가능)**
	UPROPERTY(EditDefaultsOnly, Category = "Stealing")
	float StealDuration = 10.0f;
};
