// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BulletPoolManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINIPROJECT_API UBulletPoolManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBulletPoolManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	int32 poolSize = 30;
	
	UPROPERTY()
	TArray<class ABulletPlayerBasic*> bulletPool;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletPlayerBasic> bulletClass;
	
	void InitializeBulletPool();
	
	void ReturnToPool(class ABulletPlayerBasic* returnBullet);
	
	class ABulletPlayerBasic* GetBullet(const FTransform& transform);
};
