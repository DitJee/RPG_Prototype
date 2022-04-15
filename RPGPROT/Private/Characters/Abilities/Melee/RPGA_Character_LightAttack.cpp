// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/Melee/RPGA_Character_LightAttack.h"

URPGA_Character_LightAttack::URPGA_Character_LightAttack()
{
	
	AbilityID = ERPAbilityInputID::LightAttack;
	AbilityInputID = ERPAbilityInputID::LightAttack;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Melee.LightAttack")));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Melee.LightAttack")));
}

void URPGA_Character_LightAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URPGA_Character_LightAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
