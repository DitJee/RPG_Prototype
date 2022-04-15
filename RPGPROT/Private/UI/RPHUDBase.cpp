// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RPHUDBase.h"
#include "UI/RPHUDWidget.h"


ARPHUDBase::ARPHUDBase()
{

}

void ARPHUDBase::BeginPlay()
{
	/** Create the UIHUD reference*/
	if (WBP_UIHUD)
	{
		m_W_UIHUD = CreateWidget<URPHUDWidget>(GetWorld(), WBP_UIHUD);
	}
}

URPHUDWidget* ARPHUDBase::GetUIHUD()
{
	if (m_W_UIHUD)
	{
		return m_W_UIHUD;
	}

	m_W_UIHUD = CreateWidget<URPHUDWidget>(GetWorld(), WBP_UIHUD);
	return m_W_UIHUD;
}

void ARPHUDBase::ShowUIHUD(bool bIsShown)
{
	if (m_W_UIHUD)
	{
		if (bIsShown)
		{
			m_W_UIHUD->AddToViewport();
		}
		else
		{
			m_W_UIHUD->RemoveFromViewport();
		}
	}
}

void ARPHUDBase::SetUIHUDAttributes(ARPPlayerState* PS)
{
	/**
	* Get Player attributes from PlayerState
	*/
	if (!PS)
	{
		return;
	}

	/**
	* Set the Attributes
	*/
	URPHUDWidget* UIHUD = GetUIHUD();
	if (UIHUD)
	{
		UIHUD->SetCurrentHealth(PS->GetHealth());
		UIHUD->SetMaxHealth(PS->GetMaxHealth());
		UIHUD->SetHealthPercentage(PS->GetHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
		UIHUD->SetCurrentMana(PS->GetMana());
		UIHUD->SetMaxMana(PS->GetMaxMana());
		UIHUD->SetManaPercentage(PS->GetMana() / FMath::Max<float>(PS->GetMaxMana(), 1.f));
		UIHUD->SetCurrentStamina(PS->GetStamina());
		UIHUD->SetMaxStamina(PS->GetMaxStamina());
		UIHUD->SetStaminaPercentage(PS->GetStamina() / FMath::Max<float>(PS->GetMaxStamina(), 1.f));
	}
}
