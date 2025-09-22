// TTWeaponBase.cpp

#include "Weapon/TTWeaponBase.h"

ATTWeaponBase::ATTWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
}

void ATTWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}