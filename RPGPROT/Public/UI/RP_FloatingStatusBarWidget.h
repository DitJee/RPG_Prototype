// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RP_FloatingStatusBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API URP_FloatingStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	* UMG Binder
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* CharacterText;
public:
	/**
	* Attribute setters
	*/
	UFUNCTION()
		void SetHealthPercentage(float HealthPercentage);

	UFUNCTION()
		void SetStaminaPercentage(float StaminaPercentage);

	UFUNCTION()
		void SetCharacterName(FText Name);
};
