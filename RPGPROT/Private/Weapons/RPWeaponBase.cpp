// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RPWeaponBase.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Characters/RPCharacterBase.h"
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "GameplayEffect.h"


// Sets default values
ARPWeaponBase::ARPWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USphereComponent>(FName("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
	DefaultSceneRoot->SetSphereRadius(4.0f);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
	WeaponMesh->SetupAttachment(DefaultSceneRoot);

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleCollision"));
	CapsuleCollision->SetupAttachment(DefaultSceneRoot);
	CapsuleCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CapsuleCollision->SetRelativeLocation(
		FVector(
			0.0f,
			0.0f,
			50.0f
		)
	);

	CapsuleCollision->OnComponentBeginOverlap.AddDynamic(this, &ARPWeaponBase::OnBeginOverlap);
	CapsuleCollision->OnComponentEndOverlap.AddDynamic(this, &ARPWeaponBase::OnEndOverlap);

	bIsAttacking = false;
	bIsReadyForExec = true;
}


// Called when the game starts or when spawned
void ARPWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (CapsuleCollision)
	{
		CapsuleCollision->IgnoreActorWhenMoving(
			nullptr,
			true
		);
	}
}

void ARPWeaponBase::WeaponBeginAttack(FGameplayTag EventTag)
{
	AttackEventTag = EventTag;

	bIsAttacking = true;

	if (CapsuleCollision)
	{
		CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ARPWeaponBase::WeaponEndAttack()
{
	bIsAttacking = false;

	// set the delay to make sure that begin and end event are not happen at the same time
	GetWorldTimerManager().SetTimer(CollisionHandle, this, &ARPWeaponBase::DisableCollision, 0.2f, false);
}

void ARPWeaponBase::DisableCollision()
{
	if (CapsuleCollision)
	{
		CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ARPWeaponBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/**
		Check if the weapon is hitting myself
	*/
	
	if (GetInstigator() == OtherActor || OtherActor == nullptr)
	{
		return;
	}

	// if ((GetInstigator()->GetClass() != OtherActor->GetClass()) && bIsAttacking)
	if (bIsAttacking && bIsReadyForExec && DamageEffect)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("%s() %s has execute melee attack"),
			*FString(__FUNCTION__),
			*GetInstigator()->GetName()
		);

		bIsReadyForExec = false;

		FGameplayEventData EventData;
		EventData.Instigator = GetInstigator();
		EventData.Target = OtherActor;

		ARPCharacterBase* HitActor = Cast<ARPCharacterBase>(OtherActor);

		if (HitActor == nullptr)
		{
			return;
		}

		UAbilitySystemComponent* HitActorASC = HitActor->GetAbilitySystemComponent();

		ARPCharacterBase* InstigatorActor = Cast<ARPCharacterBase>(GetInstigator());

		if (HitActorASC && InstigatorActor)
		{
			
			FGameplayEffectContextHandle EffectContext = HitActorASC->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			// Add Hit result for hit reaction calculation
			EffectContext.AddHitResult(SweepResult);

			DamageEffectSpecHandle = HitActorASC->MakeOutgoingSpec(
				DamageEffect,
				InstigatorActor->GetAbilityLevel(AbilityInputID),
				EffectContext
			);

			

			HitActorASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetInstigator(),
				AttackEventTag,
				EventData
			);

		}

			
		// TODO: Consume consumable
		
	}
}



void ARPWeaponBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	bIsReadyForExec = true;
		
	
}

