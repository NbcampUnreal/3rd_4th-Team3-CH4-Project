// TTCharacterThief.cpp

#include "Character/TTCharacterThief.h"
#include "Character/DataAsset/TTCharacterThiefData.h"

ATTCharacterThief::ATTCharacterThief()
{
	
}

void ATTCharacterThief::BeginPlay()
{
	Super::BeginPlay();

	if(ThiefMeshData && ThiefMeshData->SkeltalMeshes.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, ThiefMeshData->SkeltalMeshes.Num()-1);
		GetMesh()->SetSkeletalMesh(ThiefMeshData->SkeltalMeshes[Index]);
	}
}
