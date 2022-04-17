// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEffectVolumes/RPBuffVolume.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Characters/RPCharacterBase.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"

ARPBuffVolume::ARPBuffVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Box = CreateDefaultSubobject<UBoxComponent>(FName("Box"));
	Box->SetupAttachment(DefaultSceneRoot);

	Text = CreateDefaultSubobject<UTextRenderComponent>(FName("Text"));
	Text->SetText(FText::FromString("Buff"));
	Text->SetupAttachment(DefaultSceneRoot);
}


void ARPBuffVolume::BeginPlay()
{
	Super::BeginPlay();

	// Set a timer to buff the overlaped actor every second
	GetWorldTimerManager().SetTimer(BuffHandle, this, &ARPBuffVolume::BuffActor, 1.f, true);
}


void ARPBuffVolume::BuffActor()
{

	TArray<AActor*> OverlappingActors;
	if (Box)
	{
		Box->GetOverlappingActors(OverlappingActors);
	}

	if (OverlappingActors.Num() > 0)
	{
		/** apply damage to actors*/
		for (AActor* Actor : OverlappingActors)
		{
			ARPCharacterBase* Character = Cast<ARPCharacterBase>(Actor);
			if (Character && Character->IsAlive() && Character->GetAbilitySystemComponent())
			{
				UAbilitySystemComponent* ASC = Character->GetAbilitySystemComponent();

				FGameplayEffectContextHandle GE_ContextHandle = ASC->MakeEffectContext();

				if (GE_Buff)
				{
					ASC->ApplyGameplayEffectToSelf(
						GE_Buff.GetDefaultObject(),
						1.0f,
						GE_ContextHandle
					);
				}

			}

		}
	}
}




