// TTCharacterThiefMeshData.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TTCharacterThiefData.generated.h"

UCLASS()
class TEAM03_API UTTCharacterThiefData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Meshes")
	TArray<USkeletalMesh*> SkeltalMeshes;
};
