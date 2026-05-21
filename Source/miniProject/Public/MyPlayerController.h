// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEnemyBossSpawnIndicator> bossSpawnIndicatorClass;
	UEnemyBossSpawnIndicator* BossSpawnIndicatorInstance;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class URestartUI> restartUIClass;
	UPROPERTY()
	class URestartUI* restartUIInstance;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UEnemyBossHpUI> bossHpUIClass;
	UPROPERTY()
	class UEnemyBossHpUI* bossHpUIInstance;

	void ShowBossSpawnWarning();
	void HideBossSpawnWarning();
	
	void ShowRestartUI();

	// 보스 체력바 제어 함수
	void ShowBossHPBar(FString BossName);
	void UpdateBossHP(float CurrentHP, float MaxHP);
	void HideBossHPBar();
};
