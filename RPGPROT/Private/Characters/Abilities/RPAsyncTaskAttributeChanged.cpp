// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/RPAsyncTaskAttributeChanged.h"

URPAsyncTaskAttributeChanged* URPAsyncTaskAttributeChanged::ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
	/** 
		Create and populate new async task
	*/
	URPAsyncTaskAttributeChanged* WaitForAttributeChangedTask = NewObject<URPAsyncTaskAttributeChanged>();

	if (!IsValid(AbilitySystemComponent) || !Attribute.IsValid())
	{
		WaitForAttributeChangedTask->RemoveFromRoot();
		return nullptr;
	}

	WaitForAttributeChangedTask->ASC = AbilitySystemComponent;
	WaitForAttributeChangedTask->AttributeToListenFor = Attribute;

	// bind attribute changes to AttributeChanged() of WaitForAttributeChangedTask
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(
		WaitForAttributeChangedTask,
		&URPAsyncTaskAttributeChanged::AttributeChanged
	);

	return WaitForAttributeChangedTask;
}

URPAsyncTaskAttributeChanged* URPAsyncTaskAttributeChanged::ListenForAttributesChange(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes)
{
	/**
		Create and populate new async task
	*/
	URPAsyncTaskAttributeChanged* WaitForAttributesChangedTask = NewObject<URPAsyncTaskAttributeChanged>();

	if (!IsValid(AbilitySystemComponent) || Attributes.Num() < 1)
	{
		WaitForAttributesChangedTask->RemoveFromRoot();
		return nullptr;
	}

	WaitForAttributesChangedTask->ASC = AbilitySystemComponent;
	WaitForAttributesChangedTask->AttributesToListenFor = Attributes;

	for (FGameplayAttribute Attribute : Attributes)
	{
		// bind attribute changes to AttributeChanged() of WaitForAttributeChangedTask
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(
			WaitForAttributesChangedTask,
			&URPAsyncTaskAttributeChanged::AttributeChanged
		);
	}

	return WaitForAttributesChangedTask;
}

void URPAsyncTaskAttributeChanged::AttributeChanged(const FOnAttributeChangeData& Data)
{
	OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}

void URPAsyncTaskAttributeChanged::EndTask()
{
	if (ASC)
	{
		
		ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);
		

		for (FGameplayAttribute Attribute : AttributesToListenFor)

		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
		}
	}

	SetReadyToDestroy();
	MarkAsGarbage();
	
}


