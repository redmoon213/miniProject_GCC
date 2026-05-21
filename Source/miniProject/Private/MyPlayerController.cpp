// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnemyBoss.h"
#include "EnemyBossHpUI.h"
#include "EnemyBossSpawnIndicator.h"
#include "RestartUI.h"

void AMyPlayerController::ShowBossSpawnWarning()
{
	if (bossSpawnIndicatorClass)
	{
		BossSpawnIndicatorInstance = CreateWidget<UEnemyBossSpawnIndicator>(this, bossSpawnIndicatorClass);
		BossSpawnIndicatorInstance->AddToViewport();
	}
}

void AMyPlayerController::HideBossSpawnWarning()
{
	if (BossSpawnIndicatorInstance)
	{
		BossSpawnIndicatorInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyPlayerController::ShowRestartUI()
{
	if (restartUIClass && !restartUIInstance)
	{
		restartUIInstance = CreateWidget<URestartUI>(this, restartUIClass);
	}
	
	if (restartUIInstance)
	{
		restartUIInstance->AddToViewport();
		FInputModeUIOnly inputMode;
		inputMode.SetWidgetToFocus(restartUIInstance->TakeWidget());
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		
		SetInputMode(inputMode);
		bShowMouseCursor = true;
	}
}

void AMyPlayerController::ShowBossHPBar(FString BossName)
{
	if (bossHpUIClass && !bossHpUIInstance)
	{
		bossHpUIInstance = CreateWidget<UEnemyBossHpUI>(this, bossHpUIClass);
	}

	if (bossHpUIInstance)
	{
		if (!bossHpUIInstance->IsInViewport())
		{
			bossHpUIInstance->AddToViewport();
		}
		
		bossHpUIInstance->SetBossName(BossName);
		bossHpUIInstance->StartFillingUp(2.5f); // 2.5초 동안 차오르는 애니메이션 시작
	}
}

void AMyPlayerController::UpdateBossHP(float CurrentHP, float MaxHP)
{
	if (bossHpUIInstance)
	{
		bossHpUIInstance->UpdateHPBar(CurrentHP, MaxHP);
	}
}

void AMyPlayerController::HideBossHPBar()
{
	if (bossHpUIInstance)
	{
		bossHpUIInstance->RemoveFromParent();
		bossHpUIInstance = nullptr;
	}
}
