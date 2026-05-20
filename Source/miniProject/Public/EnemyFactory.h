// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFactory.generated.h"

UCLASS()
class MINIPROJECT_API AEnemyFactory : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> enemy;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemyBoss> enemyBoss;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemySpawnIndicator> spawnIndicator;
	
	UPROPERTY(EditAnywhere)
	class UDecalComponent* bossSpawnDecal;
	UPROPERTY(EditAnywhere)
	class USceneComponent* sceneComp;
	float minSpawnDistance = 300.0f;
	float maxSpawnDistance = 1000.0f;
	int32 enemiesSpawnLeft;
	
	FTimerHandle enemiesSpawnTimerHandle;
	
	
	
	bool GetValidSpawnLocation(FVector& outLocation);
	
	void SpawnStart(int32 enemyCount);
	
	void SpawnSingleEnemy();
	
	void SpawnEnemyBoss();
	
	float spawnTimer = 2.0f;
	
private:
	
	float currentTime = 0.0f;
};
