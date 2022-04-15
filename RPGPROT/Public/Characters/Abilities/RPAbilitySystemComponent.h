// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPAbilitySystemComponent.generated.h"

class URPGameplayAbility;
/**
 * 
 */
UCLASS()
class RPGPROT_API URPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool bCharacterAbilitiesGiven = false;
	bool bStartupEffectsApplied = false;

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<URPGameplayAbility*>& ActiveAbilities);
};
