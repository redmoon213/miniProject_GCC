// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPoolManager.h"

#include "BulletPlayerBasic.h"


// Sets default values for this component's properties
UBulletPoolManager::UBulletPoolManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBulletPoolManager::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeBulletPool();
	// ...
	
}


// Called every frame
void UBulletPoolManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBulletPoolManager::InitializeBulletPool()
{
	for (int i =0; i < poolSize; i++)
	{
		ABulletPlayerBasic* spawnBullet = GetWorld()->SpawnActor<ABulletPlayerBasic>(bulletClass);
		if (spawnBullet)
		{
			spawnBullet->SetOwnerPool(this);
			spawnBullet->SetActorHiddenInGame(true);
			spawnBullet->SetActorEnableCollision(false);
			spawnBullet->SetActorTickEnabled(false);
	
			ReturnToPool(spawnBullet);
		}
	}
}

void UBulletPoolManager::ReturnToPool(ABulletPlayerBasic* returnBullet)
{
	
	bulletPool.Push(returnBullet);
}

class ABulletPlayerBasic* UBulletPoolManager::GetBullet(const FTransform& transform)
{

	if (bulletPool.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool is Empty!"));
		return nullptr;
	}
	ABulletPlayerBasic* targetBullet = bulletPool.Pop(false);
	if (targetBullet)
	{
		targetBullet->SetActorLocationAndRotation(transform.GetLocation(), transform.GetRotation());
		targetBullet->ActivateBullet();
	}	
	
	return targetBullet;
}
