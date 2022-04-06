// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// GAS includes
#include "AbilitySystemInterface.h"

#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"

#include "RPGPROT/RPGPROT.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPWeaponBase.generated.h"

class ARPHeroCharacter;
class USkeletalMeshComponent;
class URPGameplayAbility;

UCLASS()
class RPGPROT_API ARPWeaponBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	ARPWeaponBase();

	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/**
		Whether or not to spawn this weapon with collision enabled (pickup mode).
		Set to false when spawning directly into a player's inventory or true when spawning into the world in pickup mode.
	*/
	UPROPERTY(BlueprintReadWrite)
		bool bSpawnWithCollision;

	/**
		This tag is primarily used by the first person Animation Blueprint to determine which animations to play
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RPGPROT | RPWeapon")
		FGameplayTag WeaponTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RPGPROT | RPWeapon")
		FGameplayTagContainer RestrictedPickupTags;

	/** 
		Weapon Infos
	*/
	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FText ItemName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FText ItemDescription;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FSlateBrush ItemIcon;

protected:
	UPROPERTY()
		class URPAbilitySystemComponent* AbilitySystemComponent;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
