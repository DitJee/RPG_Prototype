// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPPlayerController.h"
#include "Player/RPPlayerState.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"

void ARPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ARPPlayerState* PS = GetPlayerState<ARPPlayerState>();

	if (PS)
	{
		// TODO init ability actor info
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}