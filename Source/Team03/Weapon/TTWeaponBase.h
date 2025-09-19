// TTWeaponBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TTWeaponBase.generated.h"

UCLASS()
class TEAM03_API ATTWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ATTWeaponBase();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;
};