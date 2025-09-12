#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TTAIDataAsset.generated.h"

UCLASS()
class TEAM03_API UTTAIDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Meshes")
	TArray<USkeletalMesh*> SkeltalMeshes;
	
};
