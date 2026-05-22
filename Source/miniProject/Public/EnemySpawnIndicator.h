// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnIndicator.generated.h"

UCLASS()
class MINIPROJECT_API AEnemySpawnIndicator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawnIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	
	UPROPERTY(EditAnywhere)
	class UDecalComponent* decalComp;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> enemy;
	
	class UMaterialInstanceDynamic* dynamicMaterialInstance;
	
	float chargePercent = 0.0f;
	float spawnTime = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float maxSpawnTime = 1.0f;
};
