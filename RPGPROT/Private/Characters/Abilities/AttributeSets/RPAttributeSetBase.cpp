// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/AttributeSets/RPAttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include <RPGPROT/Public/Characters/RPCharacterBase.h>

URPAttributeSetBase::URPAttributeSetBase()
{
	// Cache tags
	HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"));
	HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"));
	HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"));
	HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"));
}

void URPAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	/**
		The function is called when the attribute is changed.
		So, for max health, mana, and stamina, we want to scale them to match the change
	*/ 

	Super::PreAttributeChange(Attribute, NewValue);

	/** 
		If a Max value changes, adjust current to keep Current % of Current to Max
	*/

	// The 'Get{Name}Attribute' is generated from the declared MACRO in the header file
	if (Attribute == GetMaxHealthAttribute()) // Health
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute()) // Mana
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute()) // Stamina
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMoveSpeedAttribute()) // Speed
	{
		NewValue = FMath::Clamp<float>(NewValue, 150.f, 1000.f);
	}
}

void URPAttributeSetBase::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();

	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyZero(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		/** 
			Change the current value to maintain the current Val/Max Percent
		*/

		const float CurrentValue = AffectedAttribute.GetCurrentValue();

		float NewDelta = 0.0f;

		if (CurrentMaxValue > 0.0f)
		{
			NewDelta = (CurrentValue * (NewMaxValue / CurrentMaxValue)) - CurrentValue;
		}
		else
		{
			NewDelta = NewMaxValue;
		}

		// apply modification to the attribute	
		AbilityComp->ApplyModToAttributeUnsafe(
			AffectedAttributeProperty,
			EGameplayModOp::Additive,
			NewDelta
			);
	}
}

void URPAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();

	/** Get instigator*/
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();

	/** Get Gameplay Tag Container*/
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	/** Get the Target actor, which should be our owner */
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ARPCharacterBase* TargetCharacter = nullptr;

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ARPCharacterBase>(TargetActor);
	}

	/** Get the Source actor */
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ARPCharacterBase* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<ARPCharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ARPCharacterBase>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		/**
			Store a local copy of the amount of damage done
			and clear the damage attribute
		*/
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			/**
				If character was alive before damage is added, handle damage.
				This prevents damage being added to dead things and replaying death animations
			*/
			bool WasAlive = true;

			if (TargetCharacter)
			{
				WasAlive = TargetCharacter->IsAlive();
			}

			if (!TargetCharacter->IsAlive())
			{
				UE_LOG(LogTemp, Warning,
					TEXT("%s() %s is NOT alive when receiving damage"),
					*FString(__FUNCTION__),
					*TargetCharacter->GetName()
				);
			}

			// Apply the health change and then clamp it
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			

			if (TargetCharacter && WasAlive)
			{
				// Log damage received.
				UE_LOG(LogTemp, Log,
					TEXT("%s() %s Damage Received: %f"),
					*FString(__FUNCTION__),
					*GetOwningActor()->GetName(),
					LocalDamageDone
				);


				/** 
					Get Hit result to determine the hit direction
				*/
				const FHitResult* HitResult = Data.EffectSpec.GetContext().GetHitResult();
				if (HitResult)
				{
					FVector ImpactVector = HitResult->Location;
					ERPHitReactDirection HitDirection = TargetCharacter->GetHitReactDirection(ImpactVector);

					switch (HitDirection)
					{
						case ERPHitReactDirection::None:
							TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
							break;
						case ERPHitReactDirection::Left:
							TargetCharacter->PlayHitReact(HitDirectionLeftTag, SourceCharacter);
							break;
						case ERPHitReactDirection::Right:
							TargetCharacter->PlayHitReact(HitDirectionRightTag, SourceCharacter);
							break;
						case ERPHitReactDirection::Front:
							TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
							break;
						case ERPHitReactDirection::Back:
							TargetCharacter->PlayHitReact(HitDirectionBackTag, SourceCharacter);
							break;
					}
				}
				else
				{
					TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
				}


				// Show damage number for the Source player unless it was self damage
				if (SourceActor != TargetActor)
				{
					// TODO: Show damage widget
				}

				if (!TargetCharacter->IsAlive())
				{
					// TODO: Give gold and XP
				}
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		/**
			Handle other health changes.
			Health loss should go through Damage.
		*/

		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		/**
			Handle mana changes.
		*/

		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		/**
			Handle stamina changes.
		*/
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}

}
void URPAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, ManaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, StaminaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, XPBounty, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, Gold, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URPAttributeSetBase, GoldBounty, COND_None, REPNOTIFY_Always);
}

void URPAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, Health, OldHealth);
}

void URPAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, MaxHealth, OldMaxHealth);
}

void URPAttributeSetBase::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, HealthRegenRate, OldHealthRegenRate);
}

void URPAttributeSetBase::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, Mana, OldMana);
}

void URPAttributeSetBase::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, MaxMana, OldMaxMana);
}

void URPAttributeSetBase::OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, ManaRegenRate, OldManaRegenRate);
}

void URPAttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, Stamina, OldStamina);
}

void URPAttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, MaxStamina, OldMaxStamina);
}

void URPAttributeSetBase::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, StaminaRegenRate, OldStaminaRegenRate);
}

void URPAttributeSetBase::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, Armor, OldArmor);
}

void URPAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, MoveSpeed, OldMoveSpeed);
}

void URPAttributeSetBase::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, CharacterLevel, OldCharacterLevel);
}

void URPAttributeSetBase::OnRep_XP(const FGameplayAttributeData& OldXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, XP, OldXP);
}

void URPAttributeSetBase::OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, XPBounty, OldXPBounty);
}

void URPAttributeSetBase::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, Gold, OldGold);
}

void URPAttributeSetBase::OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URPAttributeSetBase, GoldBounty, OldGoldBounty);
}

