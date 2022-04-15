// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPPlayerController.h"
#include "Player/RPPlayerState.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "UI/RPHUDBase.h"

void ARPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ARPPlayerState* PS = GetPlayerState<ARPPlayerState>();

	if (PS)
	{
		// initialize actor's info
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);

		/** 
			Get the HUD and initialize the attributes
		*/
		ARPHUDBase* UIHUD = Cast<ARPHUDBase>(GetHUD());

		if (UIHUD)
		{
			UIHUD->SetUIHUDAttributes(PS);

			// show the UI HUD
			UIHUD->ShowUIHUD(true);
		}

	}
}