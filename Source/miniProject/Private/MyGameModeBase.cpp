// Fill out your copyright notice in the Description page of Project Settings.


#include "miniProject/Public/MyGameModeBase.h"

#include "EnemyBoss.h"
#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

void AMyGameModeBase::OnEnemyDie()
{
	/*enemyCount--;
	if (enemyCount <= 0 )
	{
		
	}*/
	UE_LOG(LogTemp, Warning, TEXT("OnEnemyDie()"));	
	StartBossSpawn();
}


void AMyGameModeBase::StartBossSpawn()
{
	pc = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (pc)
	{
		pc->ShowBossSpawnWarning();
		
		UE_LOG(LogTemp, Warning, TEXT("Show()"));	
	}
	
	GetWorld()->GetTimerManager().SetTimer(bossSpawnTimerHandle, this, &AMyGameModeBase::SpawnBoss, 5.0f,false);
}

void AMyGameModeBase::SpawnBoss()
{
	
	pc = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (pc)
	{
		FVector spawnLocation = FVector(0.0f, 0.0f, 0.0f);
		FRotator spawnRotation = FRotator::ZeroRotator;
		
		GetWorld()->SpawnActor<AEnemyBoss>(bossClass, spawnLocation, spawnRotation);
		pc->HideBossSpawnWarning();
	}
}
