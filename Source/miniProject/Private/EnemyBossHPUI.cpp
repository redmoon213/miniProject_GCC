// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossHPUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyBossHPUI::UpdateHPBar(float CurrentHP, float MaxHP)
{
	// 분모가 0이 되는 것을 방지
	if (MaxHP > 0 && BossHPBar != nullptr)
	{
		float HPPercent = CurrentHP / MaxHP;
		BossHPBar->SetPercent(HPPercent);

		// 체력에 따라 색상을 변경하는 등 추가 로직을 여기에 넣을 수 있습니다.
	}
}

void UEnemyBossHPUI::SetBossName(FString BossName)
{
	if (BossNameText != nullptr)
	{
		BossNameText->SetText(FText::FromString(BossName));
	}
}
