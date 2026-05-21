// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossHpUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyBossHpUI::StartFillingUp(float Duration)
{
	TargetFillDuration = Duration;
	CurrentAnimationTime = 0.0f;
	bIsFillingUp = true;

	if (BossHPBar != nullptr)
	{
		BossHPBar->SetPercent(0.0f);
	}
}

void UEnemyBossHpUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsFillingUp)
	{
		CurrentAnimationTime += InDeltaTime;
		
		// 0.0 ~ 1.0 사이의 진행도 계산
		float Progress = FMath::Clamp(CurrentAnimationTime / TargetFillDuration, 0.0f, 1.0f);
		
		if (BossHPBar != nullptr)
		{
			BossHPBar->SetPercent(Progress);
		}

		// 애니메이션이 끝나면 정지
		if (Progress >= 1.0f)
		{
			bIsFillingUp = false;
		}
	}
}

void UEnemyBossHpUI::UpdateHPBar(float CurrentHP, float MaxHP)
{
	// 애니메이션 중에는 실제 체력 업데이트를 잠시 무시 (끝난 후부터 반영)
	if (bIsFillingUp) return;

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
