// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPBuffVolume.generated.h"

UCLASS()
class RPGPROT_API ARPBuffVolume : public AActor
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

	/** Buff gameplay effect class reference*/
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		TSubclassOf<class UGameplayEffect> GE_Buff;

	FTimerHandle BuffHandle;

public:
	// Sets default values for this actor's properties
	ARPBuffVolume();

	UFUNCTION()
		void BuffActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
