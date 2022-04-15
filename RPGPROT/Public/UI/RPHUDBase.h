// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Player/RPPlayerState.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RPHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class RPGPROT_API ARPHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	ARPHUDBase();

	virtual void BeginPlay() override;

	class URPHUDWidget* GetUIHUD();

	void ShowUIHUD(bool bIsShown);

	void SetUIHUDAttributes(ARPPlayerState* PS);

	UPROPERTY(EditAnywhere)
		TSubclassOf<class URPHUDWidget> WBP_UIHUD;
private:

	class URPHUDWidget* m_W_UIHUD;
};
