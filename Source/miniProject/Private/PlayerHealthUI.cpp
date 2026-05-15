// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthUI.h"

#include "PlayerHealthIcon.h"
#include "Components/HorizontalBox.h"

void UPlayerHealthUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!healthBox||!healthIconClass)
	{
		return;
	}
	
	healthBox->ClearChildren();
	healthIcons.Empty();
	
	for (int32 i = 0; i<maxHealth; i++)
	{
		UPlayerHealthIcon* newIcon = CreateWidget<UPlayerHealthIcon>(GetWorld(), healthIconClass);
	
		if (newIcon)
		{
			healthBox->AddChildToHorizontalBox(newIcon);
			healthIcons.Add(newIcon);
			newIcon->SetIsFull(true);
			
		}
	}
	
	
	
}

void UPlayerHealthUI::UpdateHealthIcon(int32 currentHealth)
{
	for (int32 i = 0; i<healthIcons.Num(); i++)
	{
		if (healthIcons[i])
		{
			
			bool bShouldFull = i < currentHealth;
			healthIcons[i]->SetIsFull(bShouldFull);
		}
		
	}
}
