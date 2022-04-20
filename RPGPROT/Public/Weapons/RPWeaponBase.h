// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "RPGPROT/RPGPROT.h"


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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RPGPROT|Weapon|Damage")
		TSubclassOf<class UGameplayEffect> DamageEffect;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RPGPROT|Weapon")
		ERPAbilityInputID AbilityInputID;

	FGameplayEffectSpecHandle DamageEffectSpecHandle;

public:
	ARPWeaponBase();

	UFUNCTION()
		void DisableCollision();

	UFUNCTION(BlueprintCallable)
		virtual void WeaponBeginAttack(FGameplayTag EventTag);
	UFUNCTION(BlueprintCallable)
		virtual void WeaponEndAttack();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult &, SweepResult
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	

};
