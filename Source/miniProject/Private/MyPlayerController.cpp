// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "EnemyBoss.h"
#include "EnemyBossSpawnIndicator.h"
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
