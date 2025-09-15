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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_SelectMesh();

private:
	UPROPERTY(EditAnywhere, Category="DataAsset")
	TObjectPtr<UTTCharacterThiefData> ThiefMeshData;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SelectMesh)
	int32 Index;
};
