// TTPoliceInput.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TTPoliceInput.generated.h"

class UInputAction;

UCLASS()
class TEAM03_API UTTPoliceInput : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> MeleeAttack;
};
