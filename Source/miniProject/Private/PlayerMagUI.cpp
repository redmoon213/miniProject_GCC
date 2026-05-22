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

void UPlayerMagUI::UpdateSkillCooldown(float Percent)
{
	if (ammoRingMID)
	{
		// 스킬 쿨다운용 파라미터 이름을 "SkillPercent"로 지정합니다.
		// 머티리얼에서 이 이름의 파라미터를 사용하여 쿨다운을 표시해야 합니다.
		ammoRingMID->SetScalarParameterValue(TEXT("AmmoPercent"), Percent);
	}
}
