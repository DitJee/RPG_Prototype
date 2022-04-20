// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"

// GAS input enum
#include "RPGPROT/RPGPROT.h"



#include "Abilities/GameplayAbility.h"
#include "RPGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URPGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	URPGameplayAbility();

	/** AbilityID to check which ability to activate*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPGPROT | Ability ")
		ERPAbilityInputID AbilityInputID = ERPAbilityInputID::None;

	/**
	*	Value to associate ability with a slot without trying to activate it automatically.
	* 
	*	Passive abilities won't be tied to an input, so we need a way to generically associate abilities with slots.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPGPROT | Ability ")
		ERPAbilityInputID AbilityID = ERPAbilityInputID::None;
	
	/**
	*	Tells an ability to activate immediately when its granted. 
		Used for passive abilities and abilities forced on others.
	*/
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
		bool ActivateAbilityOnGranted = false;

	/**
		If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
		Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	*/
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
