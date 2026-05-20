// Fill out your copyright notice in the Description page of Project Settings.


#include "miniProject/Public/MyGameModeBase.h"

#include "EnemyBoss.h"
#include "EnemyFactory.h"
#include "MyPlayerController.h"
#include "Portal.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

void AMyGameModeBase::BeginPlay()
{
	eFactory = Cast<AEnemyFactory>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyFactory::StaticClass()));
	pc = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	FString currentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	
	
	if (currentLevelName == "BossFight")
	{
		if (eFactory)
		{
			eFactory->bossSpawnDecal->SetHiddenInGame(false);
			StartBossSpawn();
			
		}
	}
	
	else
	{
		if (eFactory)
		{
			eFactory->SpawnStart(enemyCount);
		}
	}
	
}


void AMyGameModeBase::OnEnemyDie()
{
	enemyCount--;
	if (enemyCount <= 0 )
	{
		AActor* foundActor = UGameplayStatics::GetActorOfClass(GetWorld(), APortal::StaticClass());
		APortal* portal = Cast<APortal>(foundActor);
		if (portal)
		{
			portal->ShowPortal();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't Found Portal"));
		}
	}
}


void AMyGameModeBase::StartBossSpawn()
{
	//pc = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (pc)
	{
		pc->ShowBossSpawnWarning();
		pc->SetViewTargetWithBlend(eFactory, 2.0f, EViewTargetBlendFunction::VTBlend_Cubic);
		
	}
	
	GetWorld()->GetTimerManager().SetTimer(bossSpawnTimerHandle, this, &AMyGameModeBase::ReturnCameraToPlayer, 3.0f, false);
	//GetWorld()->GetTimerManager().SetTimer(bossSpawnTimerHandle, this, &AMyGameModeBase::SpawnBoss, 5.0f,false);
}

void AMyGameModeBase::SpawnBoss()
{
	
	//pc = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	
	if (pc)
	{
		//FVector spawnLocation = FVector(0.0f, 0.0f, 0.0f);
		//FRotator spawnRotation = FRotator::ZeroRotator;
		
		eFactory->SpawnEnemyBoss();
		//GetWorld()->SpawnActor<AEnemyBoss>(bossClass, spawnLocation, spawnRotation);
		pc->HideBossSpawnWarning();
	}
}

void AMyGameModeBase::ReturnCameraToPlayer()
{
	APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (pc&&player)
	{
		pc->SetViewTargetWithBlend(player, 2.0f, EViewTargetBlendFunction::VTBlend_Cubic);
	}
	GetWorld()->GetTimerManager().SetTimer(bossSpawnTimerHandle, this, &AMyGameModeBase::SpawnBoss, 2.0f, false);


}