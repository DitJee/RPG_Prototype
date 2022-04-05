// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AbilitySystemInterface.h"
#include <GameplayEffectTypes.h>

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RPPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class RPGPROT_API ARPPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	ARPPlayerState();

	/** Implement IAbilitySystemInterface*/
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class URPAttributeSetBase* GetAttributeSetBase() const;

	/**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetHealthRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetManaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetShield() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetMaxShield() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetShieldRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetArmor() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		int32 GetXP() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		int32 GetXPBounty() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		int32 GetGold() const;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|RPPlayerState|Attributes")
		int32 GetGoldBounty() const;

protected:
	FGameplayTag DeadTag;
	FGameplayTag KnockedDownTag;

	UPROPERTY()
		class URPAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class URPAttributeSetBase* AttributeSetBase;

	/** Attribute changed delegate handles */
	FDelegateHandle HealthChangedDelegateHandle;

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Attribute changed callbacks */
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	/** Tag changed callbacks */
	virtual void KnockDownTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
