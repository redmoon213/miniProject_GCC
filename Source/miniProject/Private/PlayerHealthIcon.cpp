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
		// 마테리얼이 할당되어 있다면 마테리얼을 우선 사용합니다.
		if (fullMaterial)
		{
			healthIcon->SetBrushFromMaterial(fullMaterial);
		}
		else if (fullTexture)
		{
			healthIcon->SetBrushFromTexture(fullTexture);
		}
		
	}
	
	else
	{
		// 마테리얼이 할당되어 있다면 마테리얼을 우선 사용합니다.
		if (emptyMaterial)
		{
			healthIcon->SetBrushFromMaterial(emptyMaterial);
		}
		else if (emptyTexture)
		{
			healthIcon->SetBrushFromTexture(emptyTexture);
		}
	}
	return;
}
