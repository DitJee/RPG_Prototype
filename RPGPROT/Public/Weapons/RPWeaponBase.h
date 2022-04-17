// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPWeaponBase.generated.h"



UCLASS()
class RPGPROT_API ARPWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class USphereComponent* DefaultSceneRoot;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UCapsuleComponent* CapsuleCollision;

	FGameplayTag AttackEventTag;

	bool bIsAttacking;

	FTimerHandle CollisionHandle;

	bool bIsReadyForExec;

public:
	ARPWeaponBase();

	UFUNCTION()
		void DisableCollision();
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		virtual void WeaponBeginAttack(FGameplayTag EventTag);
	UFUNCTION(BlueprintCallable)
		virtual void WeaponEndAttack();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

};
