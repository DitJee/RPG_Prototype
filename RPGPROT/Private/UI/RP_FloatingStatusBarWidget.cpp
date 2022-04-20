// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RP_FloatingStatusBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "SlateBasics.h"

void URP_FloatingStatusBarWidget::SetHealthPercentage(float HealthPercentage)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(HealthPercentage);
	}
}

void URP_FloatingStatusBarWidget::SetStaminaPercentage(float StaminaPercentage)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(StaminaPercentage);
	}
}

void URP_FloatingStatusBarWidget::SetCharacterName(FText Name)
{
	if (CharacterText)
	{
		CharacterText->SetText(Name);
	}
}
