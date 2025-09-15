// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutGameUI/TTGameInstance.h"
#include "TTSessionEntryWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class TEAM03_API UTTSessionEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Setup(const FServerInfo& ServerInfo);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RoomNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerCountText;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	// '참여' 버튼 클릭 시 호출될 함수
	UFUNCTION()
	void OnJoinButtonClicked();

	UPROPERTY()
	int32 ServerIndex;

};
