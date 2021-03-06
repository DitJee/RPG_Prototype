// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// GAS includes
#include "AbilitySystemInterface.h"

#include "RPGPROT/RPGPROT.h"
#include "GameplayTagContainer.h"


#include "GameFramework/Character.h"
#include "RPCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, ERPHitReactDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ARPCharacterBase*, Character);

class URPGameplayAbility;
/**
  The base Character class for the game. 

  Everything with an AbilitySystemComponent in this game will inherit from this class.

  This class should not be instantiated and instead subclassed.
*/

UCLASS()
class RPGPROT_API ARPCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Sets default values for this character's properties */
	ARPCharacterBase(const class FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter")
		virtual bool IsAlive() const;

	virtual void Die();

	virtual void FinishDying();

	/** Implement IAbilitySystemInterface */
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** 
		Removes all CharacterAbilities. 
		Can only be called by the Server. 
		Removing on the Server will remove from Client too.
	*/
	virtual void RemoveCharacterAbilities();

	// Switch on AbilityID to return individual ability levels. Hardcoded to 1 for every ability in this project.
	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter")
		virtual int32 GetAbilityLevel(ERPAbilityInputID AbilityID) const;

	/**
	* Getters for attributes from RPAttributeSetBase
	**/

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		float GetMaxStamina() const;

	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Attributes")
		float GetMoveSpeedBaseValue() const;

	/** Returns a list of active abilities matching the specified tags. This only returns if the ability is currently running */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<URPGameplayAbility*>& ActiveAbilities);

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPCharacter|Animation")
		ERPHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);

	UFUNCTION(NetMulticast, Reliable, WithValidation, Category = "RPGPROT|RPCharacter|Animation")
		virtual void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);

	virtual void PlayHitReact_Implementation(FGameplayTag HitDirection, AActor* DamageCauser);
	virtual bool PlayHitReact_Validate(FGameplayTag HitDirection, AActor* DamageCauser);

public:
	/**
	*	Cached Hit Reaction Tag
	*/
	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RPGPROT|RPCharacter|Weapon")
		class ARPWeaponBase* CurrentWeapon;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	// Set the Hit React direction in the Animation Blueprint
	UPROPERTY(BlueprintAssignable, Category = "RPGPROT|RPCharacter")
		FCharacterBaseHitReactDelegate ShowHitReact;

	UPROPERTY(BlueprintAssignable, Category = "RPGPROT|RPCharacter")
		FCharacterDiedDelegate OnCharacterDied;

	// Death Animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPGPROT|RPCharacter|Animation")
		UAnimMontage* DeathMontage;

protected:

	/** 
		Reference to the ASC.
		It will live on the PlayerState or here 
		if the character doesn't have a PlayerState.
	*/
	UPROPERTY()
		class URPAbilitySystemComponent* AbilitySystemComponent;

	/**
		Reference to the AttributeSetBase.
		It will live on the PlayerState or here 
		if the character doesn't have a PlayerState.
	*/
	UPROPERTY()
		class URPAttributeSetBase* AttributeSetBase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPGPROT|RPCharacter")
		FText CharacterName = FText::FromString(TEXT("Jee"));

	/** 
		Default abilities for this Character.
		These will be removed on Character death 
		and added when Character respawns.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPGPROT|RPCharacter")
		TArray<TSubclassOf<class URPGameplayAbility>> CharacterAbilities;

	/**
		Default attributes for a character for initializing on spawn / respawn.
		This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPGPROT|RPCharacter|Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;

	/**These effects are only applied one time on startup*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPGPROT|RPCharacter|Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	/** 
		Grant abilities on the Server.
		The Ability Specs will be replicated to the owning client.
	*/
	virtual void AddCharacterAbilities();

	/**
		Initialize the Character's attributes. 
		Must run on Server but we run it on Client too
		so that we don't have to wait. 
		The Server's replication to the Client won't matter since
		the values should be the same.
	*/
	virtual void InitializeAttributes();

	/**
		Add startup effects
	*/
	virtual void AddStartupEffects();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
		Setters for Attributes. 

		Only use these in special cases like Respawning, 
		otherwise use a GE	tochange Attributes.

		These change the Attribute's Base Value.
	*/

	virtual void SetHealth(float Health);
	virtual void SetMana(float Mana);
	virtual void SetStamina(float Stamina);
};
