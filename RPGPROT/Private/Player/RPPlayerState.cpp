// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPPlayerState.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "Characters/Abilities/AttributeSets/RPAttributeSetBase.h"

ARPPlayerState::ARPPlayerState()
{
	/** create ability system component and set it to be replicated*/
	AbilitySystemComponent = CreateDefaultSubobject<URPAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	/**
		Replication mode is set to be 'mixed'.
		Mixed mode means the GEs only replicated to ourself, 
		not to simulated proxies. 
		
		If another PlayerState (Hero) receives a GE,
		we won't be told about it by the Server. 
		
		Attributes, GameplayTags, and GameplayCues will still replicate to us.
	*/
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	/** 
		Create a subobject of AttributeSet and add it to the owing actor of an AbilitySystemComponent
	*/
	AttributeSetBase = CreateDefaultSubobject<URPAttributeSetBase>(TEXT("AttributeSetBase"));

	/**
		Set PlayerState's NetUpdateFrequency to the same as the Character.
		Default is very low for PlayerStates and introduces perceived lag in the ability system.
		100 is probably way too high for a shipping game, you can adjust to fit your needs.
	*/
	NetUpdateFrequency = 100.f;

	/*DeadTag = FGameplayTag::RequestGameplayTag("State.Dead");
	KnockedDownTag = FGameplayTag::RequestGameplayTag("State.KnockedDown");*/

}

void ARPPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		/** 
			Get Attribute change delegate and bind it to OnChange function
		*/
		 FOnGameplayAttributeValueChange OnValueChangeHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute());

		 HealthChangedDelegateHandle = OnValueChangeHandle.AddUObject(this, &ARPPlayerState::HealthChanged);

		 /**
			Get tag change delegate and bind it to OnChange function
		 */
		 FOnGameplayEffectTagCountChanged OnTagChangeHandle = AbilitySystemComponent->RegisterGameplayTagEvent(KnockedDownTag, EGameplayTagEventType::NewOrRemoved);

		 OnTagChangeHandle.AddUObject(this, &ARPPlayerState::KnockDownTagChanged);
	}
}

void ARPPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
}

void ARPPlayerState::KnockDownTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

UAbilitySystemComponent* ARPPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URPAttributeSetBase* ARPPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

float ARPPlayerState::GetHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float ARPPlayerState::GetMaxHealth() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

float ARPPlayerState::GetHealthRegenRate() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealthRegenRate();
	}

	return 0.0f;
}

float ARPPlayerState::GetMana() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMana();
	}

	return 0.0f;
}

float ARPPlayerState::GetMaxMana() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxMana();
	}

	return 0.0f;
}

float ARPPlayerState::GetManaRegenRate() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetManaRegenRate();
	}

	return 0.0f;
}

float ARPPlayerState::GetStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStamina();
	}

	return 0.0f;
}

float ARPPlayerState::GetMaxStamina() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxStamina();
	}

	return 0.0f;
}

float ARPPlayerState::GetStaminaRegenRate() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStaminaRegenRate();
	}

	return 0.0f;
}

float ARPPlayerState::GetShield() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetShield();
	}

	return 0.0f;
}

float ARPPlayerState::GetMaxShield() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxShield();
	}

	return 0.0f;
}

float ARPPlayerState::GetShieldRegenRate() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetShieldRegenRate();
	}

	return 0.0f;
}

float ARPPlayerState::GetArmor() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetArmor();
	}

	return 0.0f;
}

float ARPPlayerState::GetMoveSpeed() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeed();
	}

	return 0.0f;
}

int32 ARPPlayerState::GetCharacterLevel() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetCharacterLevel();
	}

	return int32();
}

int32 ARPPlayerState::GetXP() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetXP();
	}

	return int32();
}

int32 ARPPlayerState::GetXPBounty() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetXPBounty();
	}

	return int32();
}

int32 ARPPlayerState::GetGold() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetGold();
	}

	return int32();
}

int32 ARPPlayerState::GetGoldBounty() const
{
	if (IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetGoldBounty();
	}

	return int32();
}
