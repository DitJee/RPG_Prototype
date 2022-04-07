// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPCharacterBase.h"
#include "Characters/RPCharacterMovementComponent.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "Characters/Abilities/AttributeSets/RPAttributeSetBase.h"
#include "Characters/Abilities/RPGameplayAbility.h"

// Sets default values
ARPCharacterBase::ARPCharacterBase(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<URPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ARPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

bool ARPCharacterBase::IsAlive() const
{
	float CurrentHealth = GetHealth();

	UE_LOG(LogTemp, Warning, TEXT("CurrentHealth: %f"), CurrentHealth);

	return CurrentHealth > 0.0f;
}

UAbilitySystemComponent* ARPCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARPCharacterBase::AddCharacterAbilities()
{
	ENetRole Role_ = GetLocalRole();
	/** check for validity */
	if (
		AbilitySystemComponent == NULL ||
		AbilitySystemComponent->bCharacterAbilitiesGiven == true
		)
	{
		return;
	}

	// Iterate through ability list to add the it to the character
	for (TSubclassOf<URPGameplayAbility>& Ability: CharacterAbilities)
	{
		URPGameplayAbility* AbilityClass = Cast<URPGameplayAbility>(Ability->GetDefaultObject());

		// Get Ability level from AbilityID
		ERPAbilityInputID CurrentAbilityID = AbilityClass->AbilityID;
		int32 CurrentAbilityLevel = GetAbilityLevel(CurrentAbilityID);

		// Get int32 of AbilityInputID
		ERPAbilityInputID CurrentAbilityInputID = AbilityClass->AbilityInputID;
		int32 InputID = static_cast<int32>(CurrentAbilityInputID);

		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(
				Ability,
				CurrentAbilityLevel,
				InputID,
				this
			)
		);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void ARPCharacterBase::RemoveCharacterAbilities()
{
	/** check for validity */
	if (
		GetLocalRole() == ROLE_Authority ||
		!IsValid(AbilitySystemComponent) ||
		!AbilitySystemComponent->bCharacterAbilitiesGiven
		)
	{
		return;
	}

	/**
		Remove any abilities added from a previous call. 
		This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	*/
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;

	for (const FGameplayAbilitySpec& Spec: AbilitySystemComponent->GetActivatableAbilities())
	{
		if (
				(Spec.SourceObject == this) && 
				CharacterAbilities.Contains(Spec.Ability->GetClass())
			)
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// remvoe the abilty one-by-one
	for (const FGameplayAbilitySpecHandle& Ability : AbilitiesToRemove)
	{
		AbilitySystemComponent->ClearAbility(Ability);
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
	
}

int32 ARPCharacterBase::GetAbilityLevel(ERPAbilityInputID AbilityID) const
{
	return 1;
}

void ARPCharacterBase::InitializeAttributes()
{
	/** 
		Check validity of AbilitySystemComponent and DefaultAttributes
	*/
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DefaultAttributes,
		GetCharacterLevel(), 
		EffectContextHandle
	);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(
			*NewHandle.Data.Get()
		);
	}


}

void ARPCharacterBase::AddStartupEffects()
{
	/** check for validity */
	if (
		GetLocalRole() == ROLE_Authority ||
		!IsValid(AbilitySystemComponent) ||
		AbilitySystemComponent->bStartupEffectsApplied
		)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> StartupEffect: StartupEffects)
	{
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
			StartupEffect,
			GetCharacterLevel(),
			EffectContextHandle
		);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				AbilitySystemComponent
			);
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

int32 ARPCharacterBase::GetCharacterLevel() const
{
	//TODO
	return 1;
}

float ARPCharacterBase::GetHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float ARPCharacterBase::GetMaxHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

float ARPCharacterBase::GetMana() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMana();
	}

	return 0.0f;
}

float ARPCharacterBase::GetMaxMana() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxMana();
	}

	return 0.0f;
}

float ARPCharacterBase::GetStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStamina();
	}

	return 0.0f;
}

float ARPCharacterBase::GetMaxStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxStamina();
	}
	return 0.0f;
}

float ARPCharacterBase::GetShield() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetShield();
	}

	return 0.0f;
}

float ARPCharacterBase::GetMaxShield() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxShield();
	}

	return 0.0f;
}

float ARPCharacterBase::GetMoveSpeed() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeed();
	}

	return 0.0f;
}

float ARPCharacterBase::GetMoveSpeedBaseValue() const
{
	if (IsValid(AttributeSetBase))
	{

		FGameplayAttribute MoveSpeedAttribute = AttributeSetBase->GetMoveSpeedAttribute();
		FGameplayAttributeData* MoveSppedAttributeData = MoveSpeedAttribute.GetGameplayAttributeData(AttributeSetBase);

		const float MoveSpeedBaseValue = MoveSppedAttributeData->GetBaseValue();

		return MoveSpeedBaseValue;
	}

	return 0.0f;
}

void ARPCharacterBase::SetHealth(float Health)
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->SetHealth(Health);
	}
}

void ARPCharacterBase::SetMana(float Mana)
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->SetHealth(Mana);
	}
}

void ARPCharacterBase::SetStamina(float Stamina)
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->SetHealth(Stamina);
	}
}

void ARPCharacterBase::SetShield(float Shield)
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->SetHealth(Shield);
	}
}