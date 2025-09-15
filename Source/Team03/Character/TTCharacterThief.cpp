// TTCharacterThief.cpp

#include "Character/TTCharacterThief.h"
#include "Character/DataAsset/TTCharacterThiefData.h"
#include "Net/UnrealNetwork.h"

ATTCharacterThief::ATTCharacterThief()
{
	bReplicates = true;
}

void ATTCharacterThief::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		if(ThiefMeshData && ThiefMeshData->SkeltalMeshes.Num() > 0)
		{
			Index = FMath::RandRange(0, ThiefMeshData->SkeltalMeshes.Num()-1);
		}
	}
	
}

void ATTCharacterThief::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Index);
}

void ATTCharacterThief::OnRep_SelectMesh()
{
	GetMesh()->SetSkeletalMesh(ThiefMeshData->SkeltalMeshes[Index]);
}
