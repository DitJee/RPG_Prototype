// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEffectVolumes/RPDamageVolume.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Characters/RPCharacterBase.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"

ARPDamageVolume::ARPDamageVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	Box->SetupAttachment(DefaultSceneRoot);

	Text = CreateDefaultSubobject<UTextRenderComponent>(FName("Text"));
	Text->SetText(FText::FromString("Damage/s"));
	Text->SetupAttachment(DefaultSceneRoot);

}

void ARPDamageVolume::BeginPlay()
{
	Super::BeginPlay();
	
	// Set a timer to damage the overlaped actor every second
	GetWorldTimerManager().SetTimer(DamageHandle, this, &ARPDamageVolume::DamageActor, 1.f, true);
}

void ARPDamageVolume::DamageActor()
{
	TArray<AActor*> OverlappingActors;
	if (Box)
	{
		Box->GetOverlappingActors(OverlappingActors);
	}

	if (OverlappingActors.Num() > 0)
	{
		/** apply damage to actors*/
		for (AActor* Actor: OverlappingActors)
		{
			ARPCharacterBase* Character = Cast<ARPCharacterBase>(Actor);
			if (Character && Character->IsAlive() && Character->GetAbilitySystemComponent())
			{
				UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();

				FGameplayEffectContextHandle GE_ContextHandle = ASC->MakeEffectContext();

				if (GE_Damage)
				{
					FGameplayEffectSpecHandle GE_SpecHandle = ASC->MakeOutgoingSpec(
						GE_Damage,
						1.0f,
						GE_ContextHandle
					);

					
					FGameplayEffectSpecHandle GE_DamageSpecHandle = UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude
					(
						GE_SpecHandle,
						FGameplayTag::RequestGameplayTag(FName("Data.Damage")),
						Damage
					);

					ASC->ApplyGameplayEffectSpecToSelf(*GE_DamageSpecHandle.Data.Get());
				}
				
			}

		}
	}

	
}



