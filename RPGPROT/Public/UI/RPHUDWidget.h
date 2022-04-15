// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AttributeSet.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RPGPROT_API URPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* HealthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ManaText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* StaminaText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* ManaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadOnly, EditAnyWhere, Category = "RPGPROT | RPHUDWidget | Attributes")
		TArray<FGameplayAttribute> Attributes;

	class URPAsyncTaskAttributeChanged* AsyncTaskAttributeChanged;

	class URPAttributeSetBase* AttributeSetBase;

private:

	float m_CurrentHealth;
	float m_CurrentMana;
	float m_CurrentStamina;

	float m_MaxHealth;
	float m_MaxMana;
	float m_MaxStamina;

public:
	UFUNCTION()
		void ExecuteOnAttributeChanged(FGameplayAttribute Attribute, float NewValue, float OldValue);
	/**
	* Attribute setters
	*/
	UFUNCTION()
		void SetHealth();

	UFUNCTION()
		void SetMana();

	UFUNCTION()
		void SetStamina();

	UFUNCTION(BlueprintCallable)
		void SetMaxHealth(float MaxHealth);

	UFUNCTION(BlueprintCallable)
		void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintCallable)
		void SetHealthPercentage(float HealthPercentage);

	UFUNCTION(BlueprintCallable)
		void SetMaxMana(float MaxMana);

	UFUNCTION(BlueprintCallable)
		void SetCurrentMana(float CurrentMana);

	UFUNCTION(BlueprintCallable)
		void SetManaPercentage(float ManaPercentage);

	UFUNCTION(BlueprintCallable)
		void SetMaxStamina(float MaxStamina);

	UFUNCTION(BlueprintCallable)
		void SetCurrentStamina(float CurrentStamina);

	UFUNCTION(BlueprintCallable)
		void SetStaminaPercentage(float StaminaPercentage);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
};
