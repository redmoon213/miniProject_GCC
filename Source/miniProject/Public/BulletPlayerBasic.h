// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletPlayerBasic.generated.h"

UCLASS()
class MINIPROJECT_API ABulletPlayerBasic : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABulletPlayerBasic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	
	float moveSpeed = 500.f;
	
	
	UFUNCTION()
	void OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
