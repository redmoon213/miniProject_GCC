// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnemyBoss.h"
#include "EnemyBossHpUI.h"
#include "EnemyBossSpawnIndicator.h"
#include "RestartUI.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::ShowBossSpawnWarning()
{
	if (bossSpawnIndicatorClass)
	{
		BossSpawnIndicatorInstance = CreateWidget<UEnemyBossSpawnIndicator>(this, bossSpawnIndicatorClass);
		BossSpawnIndicatorInstance->AddToViewport();
	}

	// 보스 스폰 경고 사운드를 재생합니다.
	if (bossSpawnWarningSound)
	{
		bossSpawnWarningAudioComp = UGameplayStatics::SpawnSound2D(this, bossSpawnWarningSound);
	}
}

void AMyPlayerController::HideBossSpawnWarning()
{
	if (BossSpawnIndicatorInstance)
	{
		BossSpawnIndicatorInstance->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 보스 스폰 경고 사운드를 중지합니다.
	if (bossSpawnWarningAudioComp && bossSpawnWarningAudioComp->IsPlaying())
	{
		bossSpawnWarningAudioComp->Stop();
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

void AMyPlayerController::SetMenuInputMode(UUserWidget* MenuWidget)
{
	if (MenuWidget)
	{
		// 입력 모드를 UI 전용으로 설정합니다.
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(MenuWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);

		// 마우스 커서를 보이게 설정합니다.
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
