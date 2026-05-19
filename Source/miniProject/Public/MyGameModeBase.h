// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MINIPROJECT_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere)
	int32 enemyCount = 5;
	
	void OnEnemyDie();
	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyBoss> bossClass;
	
	FTimerHandle bossSpawnTimerHandle;
	
	class AMyPlayerController* pc;
	
	void StartBossSpawn();
	void SpawnBoss();
	
};
