// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/GameplayEffectExecutionCalculations/RPDamageExecCalculation.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "Characters/Abilities/AttributeSets/RPAttributeSetBase.h"

/**
	Declare the attributes to capture and define how we want to capture them from the Source and Target.
*/
struct RPDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	RPDamageStatics()
	{
		/**
			Snapshot happens at time of GESpec creation.

			We're not capturing anything from the Source in this example, 
			but there could be like AttackPower attributes that you might want.
		*/

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPAttributeSetBase, Damage, Source, true);

		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPAttributeSetBase, Armor, Target, false);
	}
};

static const RPDamageStatics& DamageStatics()
{
	static RPDamageStatics DStatics;
	return DStatics;
}

URPDamageExecCalculation::URPDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void URPDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	/** 
		Get Target and source ASCs
	*/
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	/** 
		Get Target and source actors
	*/
	AActor* TargetActor = nullptr;
	AActor* SourceActor = nullptr;
	
	if (TargetAbilitySystemComponent)
	{
		TargetActor = TargetAbilitySystemComponent->GetAvatarActor();
	}

	if (SourceAbilitySystemComponent)
	{
		SourceActor = SourceAbilitySystemComponent->GetAvatarActor();
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	/** Gather the tags from the sourceand target as that can affect which buffs should be used */
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	/** Get armor value*/
	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorDef, 
		EvaluationParameters,
		Armor
	);
	Armor = FMath::Max<float>(Armor, 0.0f);

	/** Get damage value*/
	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().DamageDef, 
		EvaluationParameters, 
		Damage
	);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(
		Spec.GetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(FName("Data.Damage")), 
			false, 
			-1.0f),
		0.0f
	);

	/**
		Calculate the resulting damage
	*/

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here

	float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor));

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				DamageStatics().DamageProperty, 
				EGameplayModOp::Additive, 
				MitigatedDamage
			)
		);
	}

	// Broadcast damages to Target ASC
	URPAbilitySystemComponent* TargetASC = Cast<URPAbilitySystemComponent>(TargetAbilitySystemComponent);
	if (TargetASC)
	{
		URPAbilitySystemComponent* SourceASC = Cast<URPAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetASC->ReceiveDamage(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}
}
