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
	class AEnemyFactory* eFactory;
	void StartBossSpawn();
	void SpawnBoss();
	
	void ReturnCameraToPlayer();
	
	// 메인 메뉴 UI 클래스를 담을 변수입니다. 에디터에서 할당해야 합니다.
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> mainMenuWidgetClass;
	
protected:
	virtual void BeginPlay() override;
};
