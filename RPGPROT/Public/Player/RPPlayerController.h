// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API ARPPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	void CreateHUD();

	class URPHUDWidget* GetHUD();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPGPROT|UI")
		TSubclassOf<class URPHUDWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "RPGPROT|UI")
		class URPHUDWidget* UIHUDWidget;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;
};
