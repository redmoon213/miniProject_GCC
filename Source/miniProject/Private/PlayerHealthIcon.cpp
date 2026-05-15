// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHealthIcon.h"

#include "Components/Image.h"


void UPlayerHealthIcon::SetIsFull(bool bIsFull)
{
	if (!healthIcon)
	{
		return;
	}
	
	if (bIsFull)
	{
		if (fullTexture)
		{
			healthIcon->SetBrushFromTexture(fullTexture);
		}
		
	}
	
	else
	{
		if (emptyTexture)
		{
			healthIcon->SetBrushFromTexture(emptyTexture);
		}
	}
	return;
}
