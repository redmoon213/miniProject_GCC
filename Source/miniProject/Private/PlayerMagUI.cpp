// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMagUI.h"

#include "Components/Image.h"

void UPlayerMagUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ammoRingImage)
	{
		ammoRingMID= ammoRingImage->GetDynamicMaterial();
		
	}
	
}


void UPlayerMagUI::UpdataAmmo(int32 currentAmmo, int32 maxAmmo)
{
	if (maxAmmo>0)
	{
		float ammoRatio = (float)currentAmmo / (float)maxAmmo;
		ammoRingMID->SetScalarParameterValue(TEXT("AmmoPercent"), ammoRatio);
	}
}
