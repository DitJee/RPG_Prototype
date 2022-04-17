// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Heros/RPHeroCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "Characters/Abilities/RPAbilitySystemComponent.h"
#include "RPGPROT/RPGPROT.h"
#include "Player/RPPlayerState.h"
#include "Animations/RPANS_JumpSection.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/RPWeaponBase.h"
#include "Engine/EngineTypes.h"

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
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
}

void ARPHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// TODO: Replicate the inventory

	/** 
		Replicate the weapon in simulate-only mode to only simulate to client. 
		Then, it will be sync when the owner when it is ready
	*/

	// TODO: Replicate currentWeapon
	//DOREPLIFETIME_CONDITION(ARPHeroCharacter, CurrentWeapon, COND_SimulatedOnly);

}

void ARPHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** 
		Bind the inputs to functions
	*/
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ARPHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ARPHeroCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &ARPHeroCharacter::Turn);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &ARPHeroCharacter::LookUp);

	/**
		Bind GAS inputs.
		Also called in OnRep_PlayerState because of a potential race condition.
	*/
	BindASCInput();

}

void ARPHeroCharacter::LookUp(float Value)
{
	if (IsAlive())
	{
		AddControllerPitchInput(Value);
	}
}

void ARPHeroCharacter::Turn(float Value)
{
	if (IsAlive())
	{
		AddControllerYawInput(Value);
	}
}

void ARPHeroCharacter::MoveForward(float Value)
{
	if (IsAlive())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		
	}
}

void ARPHeroCharacter::MoveRight(float Value)
{
	if (IsAlive())
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARPHeroCharacter::BindASCInput()
{
	if (!bASCInputBound && AbilitySystemComponent != nullptr && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(
			InputComponent,
			FGameplayAbilityInputBinds(
				FString("ConfirmTarget"),
				FString("CancelTarget"),
				FString("ERPAbilityInputID"),
				static_cast<int32>(ERPAbilityInputID::Confirm),
				static_cast<int32>(ERPAbilityInputID::Cancel)
			)
		);

		bASCInputBound = true;
	}
}

void ARPHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	return;

	// attach starting weapon to ourselves
	if (StartingWeapon)
	{

		ARPWeaponBase* Weapon = GetWorld()->SpawnActor<ARPWeaponBase>(
			StartingWeapon,
			FTransform(

			)
			);

		if (Weapon)
		{
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::SnapToTarget,
				EAttachmentRule::KeepWorld,
				true
			);

			// (AActor* ParentActor, const FAttachmentTransformRules& AttachmentRules, FName SocketName)
			Weapon->K2_AttachRootComponentTo(
				GetMesh(),
				WeaponAttachPoint,
				EAttachLocation::SnapToTarget,
				true
			);
		}
		
	}
}

void ARPHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ARPPlayerState* PS = GetPlayerState<ARPPlayerState>();

	if (PS)
	{
		/** 
			Set the ability system component on the server.
			For the client, it will be set in OnRep_PlayerState()
		*/
		AbilitySystemComponent = Cast<URPAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		/**
			Init the ability again here just to be sure
		*/
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// TODO: weapon changing handle

		/** Get attribute set*/
		AttributeSetBase = PS->GetAttributeSetBase();

		/** initialize Attribute */
		InitializeAttributes();

		/** add startup gameEffect*/
		AddStartupEffects();

		/** add character abilities*/
		AddCharacterAbilities();

		// TODO: Create HUD

		// TODO:Set attribute on respawn

		// TODO: Remove Dead Tag

	}

}

void ARPHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// TODO: Spawn inventory
}

void ARPHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ARPPlayerState* PS = GetPlayerState<ARPPlayerState>();

	if (PS)
	{
		// set ability system component on client
		AbilitySystemComponent = Cast<URPAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// init ability system component actor info on client
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		BindASCInput();

		AbilitySystemComponent->AbilityFailedCallbacks.AddUObject(this, &ARPHeroCharacter::OnAbilityActivationFailed);

		// Set AttributeSetBase
		AttributeSetBase = PS->GetAttributeSetBase();

		InitializeAttributes();
		
		// TODO: Create HUD

		// TODO: Weapon setup

		// TODO: Respawn attribute setting 

	}
}

void ARPHeroCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
}

void ARPHeroCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AbilitySystemComponent != nullptr)
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(CurrentWeaponTag);

		CurrentWeaponTag = NoWeaponTag;

		AbilitySystemComponent->AddLooseGameplayTag(CurrentWeaponTag);
	}

	Super::EndPlay(EndPlayReason);
}

void ARPHeroCharacter::OnAbilityActivationFailed(const UGameplayAbility* FailedAbility, 
	const FGameplayTagContainer& FailTags)
{
	// TODO: Weapon changing
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

void ARPHeroCharacter::JumpSectionForCombo()
{
	UAnimInstance* AI = GetMesh()->GetAnimInstance();

	if (AI && JumpSectionNs && bEnableComboPeriod)
	{
		// Get currently actived montage
		UAnimMontage* CurrentMontage = AI->GetCurrentActiveMontage();

		// get current section name
		FName SectionNameToChange = AI->Montage_GetCurrentSection();

		// Get the name of the next section
		int32 MaxIndex = JumpSectionNs->JumpSections.Num();
		FName NextSection = JumpSectionNs->JumpSections[0];

		// FName SectionNameToChange, FName NextSection, const UAnimMontage* Montage
		AI->Montage_SetNextSection(
			SectionNameToChange, 
			NextSection, 
			CurrentMontage
		);

		bEnableComboPeriod = true;
	}

}

void ARPHeroCharacter::SetHealth(float Health)
{
	Super::SetHealth(Health);
}

void ARPHeroCharacter::SetMana(float Mana)
{
	Super::SetMana(Mana);
}

void ARPHeroCharacter::SetStamina(float Stamina)
{
	Super::SetStamina(Stamina);
}

void ARPHeroCharacter::SetShield(float Shield)
{
	Super::SetShield(Shield);
}

