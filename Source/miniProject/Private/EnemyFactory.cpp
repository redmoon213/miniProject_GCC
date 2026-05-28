// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFactory.h"

#include "Enemy.h"
#include "EnemyBoss.h"
#include "EnemySpawnIndicator.h"
#include "NavigationSystem.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
AEnemyFactory::AEnemyFactory()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	sceneComp = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(sceneComp);
	bossSpawnDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("BossSpawnDecal"));
	bossSpawnDecal->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	bossSpawnDecal->SetupAttachment(sceneComp);
}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
	if (bossSpawnDecal)
	{
		bossSpawnDecal->SetHiddenInGame(true);
	}
	
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
	
	for (int i = 0; i < 20; i++) // 시도 횟수를 늘림
	{
		float randDistance = FMath::RandRange(minSpawnDistance, maxSpawnDistance);
	
		FVector randDirection = FMath::VRand();
		randDirection.Z = 0.0f;
		randDirection.Normalize();
	
		FVector targetPoint = playerLocation + (randDirection * randDistance);
	
		FNavLocation navLocation;
	
		// 허용 범위를 50으로 더 축소하여 정밀도를 높입니다.
		if (navSys->ProjectPointToNavigation(targetPoint, navLocation, FVector(50.0f, 50.0f, 100.0f)))
		{
		// ProjectPointToNavigation이 true를 반환하면 유효한 내비메쉬 지점을 찾은 것입니다.
		outLocation = navLocation.Location;

		return true;
		}
		
		}	outLocation = playerLocation + FVector(minSpawnDistance, 0.0f, 0.0f);
	return true;
}


void AEnemyFactory::SpawnStart(int32 enemyCount)
{
	enemiesSpawnLeft = enemyCount;
	
	GetWorld()->GetTimerManager().SetTimer(enemiesSpawnTimerHandle, this, 
		&AEnemyFactory::SpawnSingleEnemy, spawnTimer, true);
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
		
		// 충돌 시 절대 생성하지 않도록 가장 엄격한 옵션 적용
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
		
		// Indicator 생성
		GetWorld()->SpawnActor<AEnemySpawnIndicator>(spawnIndicator, spawnLocation, FRotator::ZeroRotator, spawnParams);
		enemiesSpawnLeft--;
	}
}

void AEnemyFactory::SpawnEnemyBoss()
{
	FVector spawnLocation = GetActorLocation();
	spawnLocation.Z = 0;
	GetWorld()->SpawnActor<AEnemyBoss>(enemyBoss, spawnLocation, FRotator::ZeroRotator);
	if (bossSpawnDecal)
	{
		bossSpawnDecal->SetHiddenInGame(true);
	}
}
