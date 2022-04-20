// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPPlayerController.h"
#include "Player/RPPlayerState.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "UI/RPHUDWidget.h"

void ARPPlayerController::CreateHUD()
{
	// create only once
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	ARPPlayerState* PS = GetPlayerState<ARPPlayerState>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<URPHUDWidget>(this, UIHUDWidgetClass);
	
	if (UIHUDWidget)
	{
		UIHUDWidget->AddToViewport();

		UIHUDWidget->SetCurrentHealth(PS->GetHealth());
		UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
		UIHUDWidget->SetHealthPercentage(PS->GetHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
		UIHUDWidget->SetCurrentMana(PS->GetMana());
		UIHUDWidget->SetMaxMana(PS->GetMaxMana());
		UIHUDWidget->SetManaPercentage(PS->GetMana() / FMath::Max<float>(PS->GetMaxMana(), 1.f));
		UIHUDWidget->SetCurrentStamina(PS->GetStamina());
		UIHUDWidget->SetMaxStamina(PS->GetMaxStamina());
		UIHUDWidget->SetStaminaPercentage(PS->GetStamina() / FMath::Max<float>(PS->GetMaxStamina(), 1.f));
	}
}

URPHUDWidget* ARPPlayerController::GetHUD()
{
	return UIHUDWidget;
}

void ARPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ARPPlayerState* PS = GetPlayerState<ARPPlayerState>();

	if (PS)
	{
		// initialize actor's info
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void ARPPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	CreateHUD();
}
