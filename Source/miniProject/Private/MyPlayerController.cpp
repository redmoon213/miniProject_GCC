// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnemyBoss.h"
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
	BossSpawnIndicatorInstance->SetVisibility(ESlateVisibility::Collapsed);
}

void AMyPlayerController::ShowRestartUI()
{
	if (restartUIClass&& !restartUIInstance)
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
