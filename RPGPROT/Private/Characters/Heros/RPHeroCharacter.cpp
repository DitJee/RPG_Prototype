// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Heros/RPHeroCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ARPHeroCharacter::ARPHeroCharacter(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bASCInputBound = false;

	/** Cache tags */
	NoWeaponTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Equipped.None"));
	WeaponChangingDelayReplicationTag = FGameplayTag::RequestGameplayTag(FName("Ability.Weapon.IsChangingDelayReplication"));
	WeaponAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Weapon"));
	CurrentWeaponTag = NoWeaponTag;
	KnockedDownTag = FGameplayTag::RequestGameplayTag("State.KnockedDown");

	// TODO: Inventory

	/** Camera Component Setup*/
	ThirdPersonCameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	ThirdPersonCameraBoom->SetupAttachment(RootComponent);
	ThirdPersonCameraBoom->bUsePawnControlRotation = true;
	ThirdPersonCameraBoom->SetRelativeLocation(FVector(0, 50, 68.492264));

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonCameraBoom);
	ThirdPersonCamera->FieldOfView = Default3PFOV;

	/** Mesh Component Setup*/
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	// TODO: Setup collision channel
	//GetMesh()->SetCollisionResponseToChannel(COLLISION_INTERACTABLE, ECollisionResponse::ECR_Overlap);
	GetMesh()->bCastHiddenShadow = true;
	GetMesh()->bReceivesDecals = false;
}

void ARPHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
}

void ARPHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}

void ARPHeroCharacter::PossessedBy(AController* NewController)
{
}

void ARPHeroCharacter::KnockDown()
{
}

void ARPHeroCharacter::PlayKnockDownEffects()
{
}

FName ARPHeroCharacter::GetWeaponAttachPoint()
{
	return FName();
}

int32 ARPHeroCharacter::GetNumWeapons() const
{
	return int32();
}

void ARPHeroCharacter::BeginPlay()
{
}

void ARPHeroCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void ARPHeroCharacter::PostInitializeComponents()
{
}

void ARPHeroCharacter::LookUp(float Value)
{
}

void ARPHeroCharacter::Turn(float Value)
{
}

void ARPHeroCharacter::MoveForward(float Value)
{
}

void ARPHeroCharacter::MoveRight(float Value)
{
}

void ARPHeroCharacter::OnRep_PlayerState()
{
}

void ARPHeroCharacter::OnRep_Controller()
{
}

void ARPHeroCharacter::BindASCInput()
{
}

void ARPHeroCharacter::SetupStartupPerspective()
{
}
