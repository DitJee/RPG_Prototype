// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Player/RPPlayerState.h"
#include "Characters/Abilities/AttributeSets/RPAttributeSetBase.h"
#include "Characters/Abilities/RPAsyncTaskAttributeChanged.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetTextLibrary.h"

void URPHUDWidget::NativeConstruct()
{
	ARPPlayerState* PS = Cast<ARPPlayerState>(GetOwningPlayer()->PlayerState);

	if (PS)
	{
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();

		AttributeSetBase = PS->GetAttributeSetBase();

		if (Attributes.Num() > 0)
		{
			/** Bind change in list of attributes to SetAttribute()*/
			AsyncTaskAttributeChanged = URPAsyncTaskAttributeChanged::ListenForAttributesChange(
				ASC,
				Attributes
			);

			if (AsyncTaskAttributeChanged)
			{
				AsyncTaskAttributeChanged->OnAttributeChanged.AddDynamic(this, &URPHUDWidget::ExecuteOnAttributeChanged);
				AsyncTaskAttributeChanged->Activate();
			}
		}
		
	}
}

void URPHUDWidget::ExecuteOnAttributeChanged(FGameplayAttribute Attribute, float NewValue, float OldValue)
{
	if (AttributeSetBase)
	{
		/* Set health widget*/
		if (Attribute == AttributeSetBase->GetHealthAttribute())
		{
			m_CurrentHealth = NewValue;
			SetHealth();
		}
		/* Set mana widget*/
		else if (Attribute == AttributeSetBase->GetManaAttribute())
		{
			m_CurrentMana = NewValue;
			SetMana();
		}
		/* Set stamina widget*/
		else if (Attribute == AttributeSetBase->GetStaminaAttribute())
		{
			m_CurrentStamina = NewValue;
			SetStamina();
		}
	}
	
}

void URPHUDWidget::SetHealth()
{
	/**
		Get FText from CurrentHealth and MaxHealth
	*/
	FText CurrentHealthText = UKismetTextLibrary::Conv_FloatToText(
		m_CurrentHealth, 
		ERoundingMode::HalfToEven,
		false,
		true, 
		1, 
		4,
		0, 
		0
	);

	FText MaxHealthText = UKismetTextLibrary::Conv_FloatToText(
		m_MaxHealth,
		ERoundingMode::HalfToEven,
		false,
		true,
		1,
		4,
		0,
		0
	);

	/** Create new Health Text value*/
	FText NewValueText = FText::Format(FText::FromString("{0} / {1}"), CurrentHealthText, MaxHealthText);

	if (HealthText)
	{
		/** Set the new Health Text value*/
		HealthText->SetText(NewValueText);
	}

	/** Calculate new health perecentage*/
	float HealthPercentage = m_CurrentHealth/FMath::Max(m_MaxHealth, 1.0f);
	SetHealthPercentage(HealthPercentage);
}

void URPHUDWidget::SetMana()
{
	/**
		Get FText from CurrentMana and MaxMana
	*/
	FText CurrentManaText = UKismetTextLibrary::Conv_FloatToText(
		m_CurrentMana,
		ERoundingMode::HalfToEven,
		false,
		true,
		1,
		4,
		0,
		0
	);

	FText MaxManaText = UKismetTextLibrary::Conv_FloatToText(
		m_MaxMana,
		ERoundingMode::HalfToEven,
		false,
		true,
		1,
		4,
		0,
		0
	);

	/** Create new mana Text value*/
	FText NewValueText = FText::Format(FText::FromString("{0} / {1}"), CurrentManaText, MaxManaText);

	if (ManaText)
	{
		/** Set the new mana Text value*/
		ManaText->SetText(NewValueText);
	}

	/** Calculate new mana perecentage*/
	float ManaPercentage = m_CurrentMana / FMath::Max(m_MaxMana, 1.0f);
	SetManaPercentage(ManaPercentage);
}

void URPHUDWidget::SetStamina()
{
	/**
	Get FText from CurrentStamina and MaxMana
*/
	FText CurrentStaminaText = UKismetTextLibrary::Conv_FloatToText(
		m_CurrentStamina,
		ERoundingMode::HalfToEven,
		false,
		true,
		1,
		4,
		0,
		0
	);

	FText MaxStaminaText = UKismetTextLibrary::Conv_FloatToText(
		m_MaxStamina,
		ERoundingMode::HalfToEven,
		false,
		true,
		1,
		4,
		0,
		0
	);

	/** Create new stamina Text value*/
	FText NewValueText = FText::Format(FText::FromString("{0} / {1}"), CurrentStaminaText, MaxStaminaText);

	if (StaminaText)
	{
		/** Set the new mana Text value*/
		StaminaText->SetText(NewValueText);
	}

	/** Calculate new mana perecentage*/
	float StaminaPercentage = m_CurrentStamina / FMath::Max(m_MaxStamina, 1.0f);
	SetStaminaPercentage(StaminaPercentage);
}

void URPHUDWidget::SetMaxHealth(float MaxHealth)
{
	m_MaxHealth = MaxHealth;
	SetHealth();
}

void URPHUDWidget::SetCurrentHealth(float CurrentHealth)
{
	m_CurrentHealth = CurrentHealth;
	SetHealth();
}

void URPHUDWidget::SetHealthPercentage(float HealthPercentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercentage);
	}
}

void URPHUDWidget::SetMaxMana(float MaxMana)
{
	m_MaxMana = MaxMana;
	SetMana();
}

void URPHUDWidget::SetCurrentMana(float CurrentMana)
{
	m_CurrentMana = CurrentMana;
	SetMana();
}

void URPHUDWidget::SetManaPercentage(float ManaPercentage)
{
	if (ManaBar)
	{
		ManaBar->SetPercent(ManaPercentage);
	}
}

void URPHUDWidget::SetMaxStamina(float MaxStamina)
{
	m_MaxStamina = MaxStamina;
	SetStamina();
}

void URPHUDWidget::SetCurrentStamina(float CurrentStamina)
{
	m_CurrentStamina = CurrentStamina;
	SetStamina();
}

void URPHUDWidget::SetStaminaPercentage(float StaminaPercentage)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(StaminaPercentage);
	}
}

void URPHUDWidget::NativeDestruct()
{
	if (AsyncTaskAttributeChanged)
	{
		AsyncTaskAttributeChanged->EndTask();
	}
}