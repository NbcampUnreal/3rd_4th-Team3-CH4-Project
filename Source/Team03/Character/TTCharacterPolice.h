// TTCharacterPolice.h

#pragma once

#include "CoreMinimal.h"
#include "Character/TTCharacterBase.h"
#include "TTCharacterPolice.generated.h"

UCLASS()
class TEAM03_API ATTCharacterPolice : public ATTCharacterBase
{
	GENERATED_BODY()
	
public:
	ATTCharacterPolice();

protected:
	virtual float GetDefaultWalkSpeed() const override;

	virtual float GetSprintWalkSpeed() const override;
};