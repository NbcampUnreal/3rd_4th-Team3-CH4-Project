// TTBaseCharacterInput.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TTBaseCharacterInput.generated.h"

class UInputAction;

UCLASS()
class TEAM03_API UTTBaseCharacterInput : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Sprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Jump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Look;
};