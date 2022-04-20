// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameModeBase.h"
#include "Player/RPPlayerController.h"
#include "Characters/Heros/RPHeroCharacter.h"

void ARPGameModeBase::HeroDied(AController* Controller)
{
	/**
	* Respawn
	*/
	if (Controller)
	{
		// TODO: Add spectator actor

		FTimerHandle RespawnTimerHandle;
		FTimerDelegate RespawnDelegate;

		RespawnDelegate = FTimerDelegate::CreateUObject(this, &ARPGameModeBase::RespawnHero, Controller);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnDelegate, RespawnDelay, false);

		// TODO: Create respawn count down widget

		/*ARPPlayerController* PC = Cast<ARPPlayerController>(Controller);
		if (PC)
		{
			PC->SetRespawnCountdown(RespawnDelay);
		}*/
	}
}

void ARPGameModeBase::RespawnHero(AController* Controller)
{
	
	// Respawn player hero
	AActor* PlayerStart = FindPlayerStart(Controller);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (HeroClass)
	{
		ARPHeroCharacter* Hero = GetWorld()->SpawnActor<ARPHeroCharacter>(
			HeroClass,
			PlayerStart->GetActorLocation(),
			PlayerStart->GetActorRotation(),
			SpawnParameters
			);

		if (Hero)
		{
			// TODO: Spectator actor

			Controller->UnPossess();
			Controller->Possess(Hero);
		}
		
	}
	
}