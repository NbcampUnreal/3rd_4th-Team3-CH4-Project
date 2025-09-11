// TTCharacterThief.h

#pragma once

#include "CoreMinimal.h"
#include "Character/TTCharacterBase.h"
#include "TTCharacterThief.generated.h"

class UTTCharacterThiefData;

UCLASS()
class TEAM03_API ATTCharacterThief : public ATTCharacterBase
{
	GENERATED_BODY()
	
public:
	ATTCharacterThief();

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="DataAsset")
	TObjectPtr<UTTCharacterThiefData> ThiefMeshData;
};
