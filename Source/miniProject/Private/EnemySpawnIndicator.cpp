// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnIndicator.h"

#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemySpawnIndicator::AEnemySpawnIndicator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	SetRootComponent(boxComp);
	
	decalComp =CreateDefaultSubobject<UDecalComponent>(TEXT("My Decal Component"));
	decalComp->SetupAttachment(boxComp);
	
	decalComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	decalComp->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void AEnemySpawnIndicator::BeginPlay()
{
	Super::BeginPlay();
	
	if (decalComp->GetDecalMaterial() != nullptr)
	{
		dynamicMaterialInstance = decalComp->CreateDynamicMaterialInstance();
	}
	
}

// Called every frame
void AEnemySpawnIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	spawnTime += DeltaTime;
	chargePercent = FMath::Clamp(spawnTime / maxSpawnTime, 0.0f, 1.0f);
	dynamicMaterialInstance->SetScalarParameterValue("chargePercent", chargePercent);
	if (spawnTime >= maxSpawnTime)
	{
		FVector spawnLocation = GetActorLocation();

		// 수정 전 코드
		/*
		if (boxComp)
		{
			spawnLocation.Z += boxComp->GetUnscaledBoxExtent().Z;
		}
		*/

		// 플레이어의 Z값을 가져와서 동일한 높이에 스폰되게 합니다.
		APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (playerPawn)
		{
			spawnLocation.Z = playerPawn->GetActorLocation().Z;
		}

		GetWorld()->SpawnActor<AEnemy>(enemy, spawnLocation, GetActorRotation());
		this->Destroy();
	}
	
}

