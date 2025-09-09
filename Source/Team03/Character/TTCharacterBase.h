// TTCharacterBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TTCharacterBase.generated.h"

UCLASS()
class TEAM03_API ATTCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATTCharacterBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
