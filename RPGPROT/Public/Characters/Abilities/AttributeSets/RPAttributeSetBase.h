// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPAttributeSetBase.generated.h"

// Macros to define getters and setters for attributes (AttributeSet.h)
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class RPGPROT_API URPAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

	URPAttributeSetBase();

public:

	// Current Health, when 0 we expect owner to die unless prevented by an ability. Capped by MaxHealth.
	// Positive changes can directly use this.
	// Negative changes to Health should go through Damage meta attribute.
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, Health)

		// MaxHealth is its own attribute since GameplayEffects may modify it
		UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, MaxHealth)

		// Health regen rate will passively increase Health every second
		UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegenRate)
		FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, HealthRegenRate)

		// Current Mana, used to execute special abilities. Capped by MaxMana.
		UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
		FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, Mana)

		// MaxMana is its own attribute since GameplayEffects may modify it
		UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
		FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, MaxMana)

		// Mana regen rate will passively increase Mana every second
		UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_ManaRegenRate)
		FGameplayAttributeData ManaRegenRate;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, ManaRegenRate)

		// Current stamina, used to execute special abilities. Capped by MaxStamina.
		UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
		FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, Stamina)

		// MaxStamina is its own attribute since GameplayEffects may modify it
		UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
		FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, MaxStamina)

		// Stamina regen rate will passively increase Stamina every second
		UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaRegenRate)
		FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, StaminaRegenRate)

		// Armor reduces the amount of damage done by attackers
		UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_Armor)
		FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, Armor)

		// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
		// Temporary value that only exists on the Server. Not replicated.
		UPROPERTY(BlueprintReadOnly, Category = "Damage")
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, Damage)

		// MoveSpeed affects how fast characters can move.
		UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
		FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, MoveSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "Character Level", ReplicatedUsing = OnRep_CharacterLevel)
		FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, CharacterLevel)

		// Experience points gained from killing enemies. Used to level up (not implemented in this project).
		UPROPERTY(BlueprintReadOnly, Category = "XP", ReplicatedUsing = OnRep_XP)
		FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, XP)

		// Experience points awarded to the character's killers. Used to level up (not implemented in this project).
		UPROPERTY(BlueprintReadOnly, Category = "XP", ReplicatedUsing = OnRep_XPBounty)
		FGameplayAttributeData XPBounty;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, XPBounty)

		// Gold gained from killing enemies. Used to purchase items (not implemented in this project).
		UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_Gold)
		FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, Gold)

		// Gold awarded to the character's killer. Used to purchase items (not implemented in this project).
		UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_GoldBounty)
		FGameplayAttributeData GoldBounty;
	ATTRIBUTE_ACCESSORS(URPAttributeSetBase, GoldBounty)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	/**
	* These OnRep functions exist to make sure that the ability system internal representations are synchronized properly during replication
	**/

	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
		virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

	UFUNCTION()
		virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
		virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
		virtual void OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate);

	UFUNCTION()
		virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
		virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
		virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate);

	UFUNCTION()
		virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
		virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);

	UFUNCTION()
		virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);

	UFUNCTION()
		virtual void OnRep_XP(const FGameplayAttributeData& OldXP);

	UFUNCTION()
		virtual void OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty);

	UFUNCTION()
		virtual void OnRep_Gold(const FGameplayAttributeData& OldGold);

	UFUNCTION()
		virtual void OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty);
	
};
