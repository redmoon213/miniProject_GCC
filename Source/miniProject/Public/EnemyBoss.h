// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBoss.generated.h"

UCLASS()
class MINIPROJECT_API AEnemyBoss : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyBoss();

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
	class USceneComponent* decalGroup;
	// --- 십자 차징 공격 관련 변수 및 컴포넌트 ---
	
	// 사방 공격 범위를 표시할 4개의 데칼 (상, 하, 좌, 우)
	UPROPERTY(EditAnywhere, Category = "Attack")
	class UDecalComponent* decalNorth;
	UPROPERTY(EditAnywhere, Category = "Attack")
	class UDecalComponent* decalSouth;
	UPROPERTY(EditAnywhere, Category = "Attack")
	class UDecalComponent* decalEast;
	UPROPERTY(EditAnywhere, Category = "Attack")
	class UDecalComponent* decalWest;

	// 각 데칼을 위한 다이내믹 머티리얼
	UPROPERTY()
	class UMaterialInstanceDynamic* matNorth;
	UPROPERTY()
	class UMaterialInstanceDynamic* matSouth;
	UPROPERTY()
	class UMaterialInstanceDynamic* matEast;
	UPROPERTY()
	class UMaterialInstanceDynamic* matWest;
	
	//회전 차지공격 구현을 위한 변수
	bool bChargeDirectionMode = false;
	FTimerHandle testHandle;
	float currentRadius = 0.0f;
	float nextRadius = 15.0f;
	
	// 차징 상태 여부
	bool bIsCharging = false;

	// 현재 차징 시간
	float currentChargeTime = 0.0f;

	// 최대 차징 시간 (공격 발동까지 걸리는 시간)
	UPROPERTY(EditAnywhere, Category = "Attack")
	float maxChargeTime = 2.0f;

	// 데칼 최대 길이
	UPROPERTY(EditAnywhere, Category = "Attack")
	float maxDecalLength = 1000.0f;

	// 데칼 폭
	UPROPERTY(EditAnywhere, Category = "Attack")
	float decalWidth = 50.0f;

	// 차징 공격 시작 함수
	void StartCrossCharge();

	// 차징 공격 업데이트 함수 (Tick에서 호출)
	void UpdateCrossCharge(float DeltaTime);

	// 차징 완료 후 공격 실행 함수
	void ExecuteCrossCharge();

	// 플레이어 참조
	class APawn* player;
	
	
	
	//투사체 발사를 위한 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletEnemyBasic> bossBulletClass;
	class ABulletEnemyBasic* bossBulletInstance;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition; 
	
	void StartProjectileBossPattern();
	void EndProjectilePattern();
	void FireProjectile();
	bool bIsProjectilePattern = false;
	FTimerHandle projectileHandle;
	
	float currentProjectileRadius = 0.0f;
	UPROPERTY(EditAnywhere)
	float increaseProjectileRadius = 3.0f;
	float maxProjectileRadius = 180.0f;
	// --- --- --- --- --- --- --- --- --- ---
};
