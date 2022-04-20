// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPPlayerState.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "Characters/Abilities/AttributeSets/RPAttributeSetBase.h"
#include "Characters/Heros/RPHeroCharacter.h"
#include "UI/RP_FloatingStatusBarWidget.h"
#include "UI/RPHUDBase.h"
#include "Player/RPPlayerController.h"
#include "UI/RPHUDWidget.h"

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

	DeadTag = FGameplayTag::RequestGameplayTag("State.Dead");
	KnockedDownTag = FGameplayTag::RequestGameplayTag("State.KnockedDown");

}

void ARPPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent && AttributeSetBase)
	{
		/** 
			Get Attribute change delegate and bind it to OnChange function
		*/

		// Health
		 HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ARPPlayerState::HealthChanged);
		 MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &ARPPlayerState::MaxHealthChanged);
		 HealthRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthRegenRateAttribute()).AddUObject(this, &ARPPlayerState::HealthRegenRateChanged);

		 // Mana
		 ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaAttribute()).AddUObject(this, &ARPPlayerState::ManaChanged);
		 MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxManaAttribute()).AddUObject(this, &ARPPlayerState::MaxManaChanged);
		 ManaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaRegenRateAttribute()).AddUObject(this, &ARPPlayerState::ManaRegenRateChanged);

		 // Stamina
		 StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddUObject(this, &ARPPlayerState::StaminaChanged);
		 MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute()).AddUObject(this, &ARPPlayerState::MaxStaminaChanged);
		 StaminaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaRegenRateAttribute()).AddUObject(this, &ARPPlayerState::StaminaRegenRateChanged);


		 /**
			Get tag change delegate and bind it to OnChange function
		 */
		 FOnGameplayEffectTagCountChanged OnTagChangeHandle = AbilitySystemComponent->RegisterGameplayTagEvent(KnockedDownTag, EGameplayTagEventType::NewOrRemoved);

		 OnTagChangeHandle.AddUObject(this, &ARPPlayerState::KnockDownTagChanged);
	}
}

void ARPPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{

	float Health = Data.NewValue;

	// Update floating status bar
	ARPHeroCharacter* Hero = Cast<ARPHeroCharacter>(GetPawn());
	if (Hero)
	{
		URP_FloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
		}
	}

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

	// If the player died, handle death
	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if (Hero)
		{
			Hero->Die();
		}
	}
}

void ARPPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;

	/**
		Update floating status bar
	*/
	ARPHeroCharacter* Hero = Cast<ARPHeroCharacter>(GetPawn());
	if (Hero)
	{
		URP_FloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetHealthPercentage(GetHealth() / MaxHealth);
		}
	}

	/**
		Update the HUD
	*/
	ARPPlayerController* PC = Cast<ARPPlayerController>(GetOwner());
	if (PC)
	{
		ARPHUDBase* HUD = Cast<ARPHUDBase>(PC->GetHUD());
		if (HUD)
		{
			URPHUDWidget* UIHUD = HUD->GetUIHUD();
			if (UIHUD)
			{
				UIHUD->SetMaxHealth(MaxHealth);
			}
		}
	}

}

void ARPPlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{

	// TODO: No widget for this
}

void ARPPlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
	float Mana = Data.NewValue;

	// Update floating status bar
	ARPHeroCharacter* Hero = Cast<ARPHeroCharacter>(GetPawn());
	if (Hero)
	{
		URP_FloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			// TODO: Mana Widget bar
			//HeroFloatingStatusBar->SetManaPercentage(Mana / GetMaxMana());
		}
	}

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

}

void ARPPlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	float MaxMana = Data.NewValue;

	/**
		Update floating status bar
	*/
	ARPHeroCharacter* Hero = Cast<ARPHeroCharacter>(GetPawn());
	if (Hero)
	{
		URP_FloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			// TODO: Mana widget bar
			//HeroFloatingStatusBar->SetManaPercentage(GetMana() / MaxMana);
		}
	}

	/**
		Update the HUD
	*/
	ARPPlayerController* PC = Cast<ARPPlayerController>(GetOwner());
	if (PC)
	{
		ARPHUDBase* HUD = Cast<ARPHUDBase>(PC->GetHUD());
		if (HUD)
		{
			URPHUDWidget* UIHUD = HUD->GetUIHUD();
			if (UIHUD)
			{
				UIHUD->SetMaxMana(MaxMana);
			}
		}
	}
	
}

void ARPPlayerState::ManaRegenRateChanged(const FOnAttributeChangeData& Data)
{
	// TODO: No widget for this
}

void ARPPlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{
	float Stamina = Data.NewValue;

	// Update floating status bar
	ARPHeroCharacter* Hero = Cast<ARPHeroCharacter>(GetPawn());
	if (Hero)
	{
		URP_FloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetStaminaPercentage(Stamina / GetMaxStamina());
		}
	}

	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

}

void ARPPlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	float MaxStamina = Data.NewValue;

	/**
		Update floating status bar
	*/
	ARPHeroCharacter* Hero = Cast<ARPHeroCharacter>(GetPawn());
	if (Hero)
	{
		URP_FloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetStaminaPercentage(GetStamina() / MaxStamina);
		}
	}

	/**
		Update the HUD
	*/
	ARPPlayerController* PC = Cast<ARPPlayerController>(GetOwner());
	if (PC)
	{
		ARPHUDBase* HUD = Cast<ARPHUDBase>(PC->GetHUD());
		if (HUD)
		{
			URPHUDWidget* UIHUD = HUD->GetUIHUD();
			if (UIHUD)
			{
				UIHUD->SetMaxStamina(MaxStamina);
			}
		}
	}
	
}

void ARPPlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
	// TODO: No widget for this
}

void ARPPlayerState::KnockDownTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// TODO: KnockDownStage
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

bool ARPPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}