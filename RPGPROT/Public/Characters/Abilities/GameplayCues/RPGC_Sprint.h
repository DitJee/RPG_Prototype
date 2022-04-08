// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "RPGC_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API ARPGC_Sprint : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	ARPGC_Sprint();

	UFUNCTION(BlueprintCallable, Category = "RPGPROT | Gameplay Cue Notify Actor | Sprint")
		void ExecuteWhileActive(AActor* MyTarget, const FGameplayCueParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "RPGPROT | Gameplay Cue Notify Actor | Sprint")
		void ExecuteOnRemove(AActor* MyTarget, const FGameplayCueParameters& Parameters);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RPGPROT | Gameplay Cue Notify Actor | Sprint | Particle System")
		class UParticleSystem* SprintFX;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RPGPROT | Gameplay Cue Notify Actor | Sprint | Particle System")
		FName AttachPointName_L;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RPGPROT | Gameplay Cue Notify Actor | Sprint | Particle System")
		FName AttachPointName_R;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RPGPROT | Gameplay Cue Notify Actor | Sprint | Particle System")
		FVector SprintFXScale;

protected:
	class UParticleSystemComponent* Particle_L;
	class UParticleSystemComponent* Particle_R;
};
