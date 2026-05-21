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
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URestartUI> restartUIClass;
	class URestartUI* restartUIInstance;
	
	void ShowBossSpawnWarning();
	void HideBossSpawnWarning();
	
	void ShowRestartUI();
};
