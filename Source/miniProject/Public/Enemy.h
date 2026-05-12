// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class MINIPROJECT_API AEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();

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
	
	UPROPERTY(EditAnywhere)
	class UMaterialInstanceDynamic* dynamicMaterial;
	
	UPROPERTY(EditAnywhere);
	int32 currentDiceEye = 5;
	
	float testTimeStack = 0.f;
	//눈금 UV 오프셋
	
	const FVector2D diceOffset[6] = {
		FVector2D(0.4f, 0.523f),	//1번 눈금 
		FVector2D(0.65f, 0.023f),   // 2번 눈금
		FVector2D(0.4f, 0.273f),    // 3번 눈금
		FVector2D(0.388f, 0.756f),	// 4번 눈금
		FVector2D(0.15, 0.023f),	// 5번 눈금, 좌상단
		FVector2D(0.4f, 0.023f)		// 6번 눈금
	};
	
	
	
	
	void UpdateDiceEye();
};
