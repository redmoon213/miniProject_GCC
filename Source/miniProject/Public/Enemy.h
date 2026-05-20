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
	class UBoxComponent* damageBoxComp;
	
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	
	UPROPERTY(EditAnywhere, Category="Appearance")
	class UMaterialInterface* materialRed;
	UPROPERTY(EditAnywhere, Category="Appearance")
	class UMaterialInterface* materialBlue;
	UPROPERTY(EditAnywhere, Category="Appearance")
	class UMaterialInterface* materialPurple;
	
	class UMaterialInterface* baseMaterial;
	
	UPROPERTY(EditAnywhere)
	class UMaterialInstanceDynamic* dynamicMaterial;
	
	UPROPERTY(EditAnywhere)
	int32 currentDiceEye = 6;
	
	FTimerHandle hitFlashTimer;
	FVector knockbackSpeed;
	
	float moveSpeed = 300.0f;
	
	FVector dir;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEnemyIndicatorWidget> indicatorWidget;
	
	class UEnemyIndicatorWidget* indicatorInstance;
	
	//눈금 텍스쳐 UV 오프셋
	const FVector2D diceOffset[6] = {
		FVector2D(0.395f, 0.49f),	//1번 눈금 
		FVector2D(0.65f, 0.023f),   // 2번 눈금
		FVector2D(0.4f, 0.256f),    // 3번 눈금
		FVector2D(0.388f, 0.756f),	// 4번 눈금
		FVector2D(0.15, 0.02f),	// 5번 눈금, 좌상단
		FVector2D(0.403f, 0.008f)		// 6번 눈금
	};
	
	// 차지공격 구현을 위한 변수및 함수 //
	UPROPERTY(EditAnywhere, Category="AttackDecal");
	class UDecalComponent* attackDecalComp;
	
	UPROPERTY(EditAnywhere)
	class UMaterialInstanceDynamic* dynamicAttackMaterial;
	
	bool isCharging = false;
	float currentChargeTime;
	float maxChargeTime;
	float currentChargeCoolTime = 0.0f;
	
	void Charging();
	void ChargingExcute();
	///////////////////////////////////////
	
	// 일반 투사체공격 구현을 위한 변수 및 함수 //
	
	bool isProjectileReady = false;
	float currentProjectileCooldown = 0.0f;
	float maxProjectileCooldown = 2.0f;
	
	
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletEnemyBasic> bulletFactory;
	
	void FireProjectile();
	
	
	////////////////////////////////////////////////////////////////
	
	APawn* player;
	
	int32 attackMode;
	
	void diceTakeDamage();
	void ResetHitFlash();
	void Knockback(FVector bulletDirection);
	
	void MoveToPlayer(float deltaTime);
	void FireProjectileMode(float deltaTime);
	void ChargingMode(float deltaTime);
	void UpdateDiceEye();
	
	UFUNCTION()
	void OnOverlapToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADroppedAmmo> droppedAmmoClass;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> debrisClass;
	void Die();
};
