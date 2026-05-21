// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossHpUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyBossHpUI::UpdateHPBar(float CurrentHP, float MaxHP)
{
	// 분모가 0이 되는 것을 방지
	if (MaxHP > 0 && BossHPBar != nullptr)
	{
		float HPPercent = CurrentHP / MaxHP;
		BossHPBar->SetPercent(HPPercent);
	}
}

void UEnemyBossHpUI::SetBossName(FString BossName)
{
	if (BossNameText != nullptr)
	{
		BossNameText->SetText(FText::FromString(BossName));
	}
}
