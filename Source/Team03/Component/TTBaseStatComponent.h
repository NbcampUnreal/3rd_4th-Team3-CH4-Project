// TTBaseStatComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TTBaseStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM03_API UTTBaseStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTTBaseStatComponent();
};
