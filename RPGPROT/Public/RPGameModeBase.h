// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API ARPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void HeroDied(AController* Controller);

	void RespawnHero(AController* Controller);

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "RPGPROT|Gamemode|Respawn Params")
		TSubclassOf<class ARPHeroCharacter> HeroClass;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "RPGPROT|Gamemode|Respawn Params")
		float RespawnDelay = 5.0f;
};
