// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPDamageVolume.generated.h"

UCLASS()
class RPGPROT_API ARPDamageVolume : public AActor
{
	GENERATED_BODY()

public:

	/** DefaultSceneRoot */
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class USceneComponent* DefaultSceneRoot;

	/** Colliding Box*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UBoxComponent* Box;

	/** Floating Text*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UTextRenderComponent* Text;

	/** Damage gameplay effect class reference*/
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		TSubclassOf<class UGameplayEffect> GE_Damage;

	/** Damage/s value*/
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		float Damage = 10.0f;

	FTimerHandle DamageHandle;

public:	
	// Sets default values for this actor's properties
	ARPDamageVolume();

	UFUNCTION()
		void DamageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
