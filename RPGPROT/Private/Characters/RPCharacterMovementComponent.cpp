// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPCharacterMovementComponent.h"
#include "Characters/RPCharacterBase.h"

URPCharacterMovementComponent::URPCharacterMovementComponent()
{

}

float URPCharacterMovementComponent::GetMaxSpeed() const
{
	ARPCharacterBase* Owner = Cast<ARPCharacterBase>(GetOwner());

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (!Owner->IsAlive())
	{
		return 0.0f;
	}

	if (RequestToStartSprinting)
	{
		return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
	}

	return Owner->GetMoveSpeed();
}

void URPCharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void URPCharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}
