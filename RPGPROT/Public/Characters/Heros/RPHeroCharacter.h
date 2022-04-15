// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <GameplayEffectTypes.h>

#include "CoreMinimal.h"
#include "Characters/RPCharacterBase.h"
#include "RPHeroCharacter.generated.h"


/**
 * 
 */
UCLASS()
class RPGPROT_API ARPHeroCharacter : public ARPCharacterBase
{
	GENERATED_BODY()

public:
	
	ARPHeroCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Called to bind functionality to input*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Only called on the Server. Calls before Server's AcknowledgePossession.*/
	virtual void PossessedBy(AController* NewController) override;

	/** Server handles knockdown - cancel abilities, remove effects, activate knockdown ability*/
	virtual void KnockDown();

	/** Plays knockdown effects for all clients from KnockedDown tag listener on PlayerState*/
	virtual void PlayKnockDownEffects();

	FName GetWeaponAttachPoint();

	UFUNCTION(BlueprintCallable, Category = "RPGPROT|Inventory")
		int32 GetNumWeapons() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPGPROT|Camera")
		float BaseTurnRate = 45.0f;;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPGPROT|Camera")
		float BaseLookUpRate = 45.0f;;

	UPROPERTY(BlueprintReadOnly, Category = "RPGPROT|Camera")
		float StartingThirdPersonCameraBoomArmLength;

	UPROPERTY(BlueprintReadOnly, Category = "RPGPROT|Camera")
		FVector StartingThirdPersonCameraBoomLocation;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RPGPROT|Camera")
		class USpringArmComponent* ThirdPersonCameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RPGPROT|Camera")
		class UCameraComponent* ThirdPersonCamera;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPGPROT|Camera")
		float Default3PFOV = 80.0f;;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RPGPROT|RPHeroCharacter")
		FName WeaponAttachPoint;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RPGPROT|RPHeroCharacter")
		TSubclassOf<UGameplayEffect> KnockDownEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "RPGPROT|RPHeroCharacter")
		TSubclassOf<UGameplayEffect> DeathEffect;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "RPGPROT | RPHeroCharacter | Combo")
		bool bEnableComboPeriod = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "RPGPROT | RPHeroCharacter | Combo")
		const class URPANS_JumpSection* JumpSectionNs;

	UFUNCTION(BlueprintCallable, Category = "RPGPROT | RPHeroCharacter | Combo")
		void JumpSectionForCombo();


	/** Cache tags */
	FGameplayTag NoWeaponTag;
	FGameplayTag WeaponChangingDelayReplicationTag;
	FGameplayTag WeaponAbilityTag;
	FGameplayTag KnockedDownTag;
	FGameplayTag CurrentWeaponTag;

	/** Tag changed delegate handles */
	FDelegateHandle WeaponChangingDelayReplicationTagChangedDelegateHandle;

	bool bASCInputBound;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PostInitializeComponents() override;

	void LookUp(float Value);

	void Turn(float Value);

	void MoveForward(float Value);

	void MoveRight(float Value);

	/** Client only */
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	void OnAbilityActivationFailed(const UGameplayAbility* FailedAbility, const FGameplayTagContainer& FailTags);

	/**
		Called from both SetupPlayerInputComponent() and OnRep_PlayerState() because of a 
		potential race condition where the PlayerController might call ClientRestart() 
		which calls SetupPlayerInputComponent() before the PlayerState is replicated to 
		the client so the PlayerState would be null in SetupPlayerInputComponent().

		Conversely, the PlayerState might be repped before the PlayerController calls 
		ClientRestart() so the Actor's InputComponent would be null in OnRep_PlayerState().
	*/
	
	void BindASCInput();

	void SetPerspective();


	virtual void SetHealth(float Health) override;
	virtual void SetMana(float Mana) override;
	virtual void SetStamina(float Stamina) override;
	virtual void SetShield(float Shield) override;
};
