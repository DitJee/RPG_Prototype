// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/RPANS_WeaponAttack.h"
#include "Characters/RPCharacterBase.h"
#include "Weapons/RPWeaponBase.h"
#include "Abilities/GameplayAbilityTypes.h"

URPANS_WeaponAttack::URPANS_WeaponAttack()
{

}

void URPANS_WeaponAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	ARPCharacterBase* Character = Cast<ARPCharacterBase>(MeshComp->GetOwner());

	if (Character)
	{
		ARPWeaponBase* Weapon = Character->CurrentWeapon;

		if (Weapon)
		{
			/** 
				Pass the AbilityInputID to the weapon which will dictate the level of the attack.
			*/
			Weapon->AbilityInputID = AbilityInputID;

			Weapon->bIsAttacking = true;

			FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Montage.Shared.WeaponHit"));

			Weapon->WeaponBeginAttack(EventTag);
		}
		
	}
}

void URPANS_WeaponAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ARPCharacterBase* Character = Cast<ARPCharacterBase>(MeshComp->GetOwner());

	if (Character)
	{
		ARPWeaponBase* Weapon = Character->CurrentWeapon;
		if (Weapon)
		{
			Weapon->WeaponEndAttack();
		}
	}
}
