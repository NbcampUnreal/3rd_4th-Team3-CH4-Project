// TTWeaponBase.cpp

#include "Weapon/TTWeaponBase.h"

ATTWeaponBase::ATTWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(WeaponMesh);
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
}

void ATTWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}