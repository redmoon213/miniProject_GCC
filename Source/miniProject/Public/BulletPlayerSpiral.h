// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletPlayerSpiral.generated.h"

UCLASS()
class MINIPROJECT_API ABulletPlayerSpiral : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABulletPlayerSpiral();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	// 회전 속도 (라디안/초)
	UPROPERTY(EditAnywhere, Category = "Spiral")
	float rotationSpeed = 8.0f;

	// 확장 속도 (바깥으로 뻗어나가는 속도)
	UPROPERTY(EditAnywhere, Category = "Spiral")
	float expansionSpeed = 400.0f;

	// 총구 위치 보정 (플레이어 발바닥 등 묻힘 방지)
	UPROPERTY(EditAnywhere, Category = "Spiral")
	float zOffset = 50.0f;

private:
	FVector centerLocation;
	float currentAngle = 0.0f;
	float currentRadius = 0.0f;
};
