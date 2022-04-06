// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RPWeaponBase.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"

// Sets default values
ARPWeaponBase::ARPWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

UAbilitySystemComponent* ARPWeaponBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ARPWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

