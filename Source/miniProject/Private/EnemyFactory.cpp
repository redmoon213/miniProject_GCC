// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFactory.h"

#include "Enemy.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyFactory::AEnemyFactory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnStart(5);
	
}

// Called every frame
void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool AEnemyFactory::GetValidSpawnLocation(FVector& outLocation)
{
	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	if (!navSys || !playerPawn)
	{
		return false;
	}
	
	FVector playerLocation = playerPawn->GetActorLocation();
	
	for (int i =0; i<10; i++)
	{
		float randDistance = FMath::RandRange(minSpawnDistance, maxSpawnDistance);
	
		FVector randDirection = FMath::VRand();
		randDirection.Z = 0.0f;
		randDirection.Normalize();
	
		FVector targetPoint = playerLocation + (randDirection * randDistance);
	
		FNavLocation navLocation;
	
		if (navSys->ProjectPointToNavigation(targetPoint, navLocation, FVector(500.0f, 500.0f, 1000.0f)))
		{
			outLocation = navLocation.Location;
		
			return true;
	
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("test>>>"));
	outLocation = playerLocation + FVector(1000.0f, 0.0f, 0.0f);
	return true;
}


void AEnemyFactory::SpawnStart(int32 enemyCount)
{
	enemiesSpawnLeft = enemyCount;
	
	GetWorld()->GetTimerManager().SetTimer(enemiesSpawnTimerHandle, this, &AEnemyFactory::SpawnSingleEnemy, spawnTimer, true);
}


void AEnemyFactory::SpawnSingleEnemy()
{
	if (enemiesSpawnLeft <= 0 )
	{
		GetWorld()->GetTimerManager().ClearTimer(enemiesSpawnTimerHandle);
		return;
	}
	
	FVector spawnLocation;
	
	if (GetValidSpawnLocation(spawnLocation))
	{
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor<AEnemy>(enemy, spawnLocation, FRotator::ZeroRotator, spawnParams);
		enemiesSpawnLeft--;
	}
	
	
}
