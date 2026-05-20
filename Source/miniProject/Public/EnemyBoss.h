// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBoss.generated.h"

UENUM(BlueprintType)
enum class EBossPattern : uint8
{
	Idle,
	Projectile,
	Charging,
	JumpAttack,
	Moving,
	MAX
};


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
	float nextRadius = 45.0f;
	
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
	
	//보스의 이동 구현
	UPROPERTY(EditAnywhere, Category = "Boss|Movement");
	float moveSpeed = 300.0f;
	UPROPERTY(EditAnywhere, Category = "Boss|Movement")
	float hopHeight = 150.0f;
	bool bIsMoving = false;
	float hopAlpha = 0.0f;
	float baseZ = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Boss|Movement")
	float hopDuration = 1.0f;
	
	void MoveStart();
	void MoveForward(float DeltaTime);
	//
	
	UPROPERTY(EditAnywhere)
	int32 maxHp;
	UPROPERTY(EditAnywhere)
	int32 currentHp;
	//플레이어를 향해 회전하기 위한 변수와 함수
	bool bCanRotate = true;
	void SearchPlayer(float DeltaTime);
	
	
	//보스의 패턴을 결정하기 위한 변수와 함수
	FTimerHandle bossPatternHandle;
	
	void ResetHitFlash();
	
	//
	UFUNCTION()
	void OnOverlapToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//보스 피격에 관련된 함수와 변수
	FTimerHandle hitFlashHandle;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
	class UMaterialInstanceDynamic* hitFlashDynamicMaterial;
	//
	
	//플레이어를 타겟으로 한 점프 공격 변수 및 함수
	void StartJumpAttack();
	void ExecuteJumpAttack();
	void UpdateJumpAttack(float DeltaTime);
	void UpdateShaking(float DeltaTime);
	bool bIsShaking = false;
	float currentShakeTime = 0.0f;
	
	UPROPERTY(EditAnywhere, Category="Boss|Attack")
	float shakeDuration = 0.6f;
	UPROPERTY(EditAnywhere, Category="Boss|Attack")
	float shakeIntensity = 4.0f;
	UPROPERTY(EditAnywhere, Category="Boss|Attack")
	float shakeSpeed = 75.0f;
	
	bool bIsJumping = false;
	FVector jumpStartLocation;
	FVector jumpTargetLocation;
	float jumpAlpha = 0.0f;
	
	UPROPERTY(EditAnywhere, Category="Boss|Attack")
	float jumpDurtation = 1.0f;
	UPROPERTY(EditAnywhere, Category="Boss|Attack")
	float jumpheigth = 400.0f;
	
	// 보스 패턴 결정을 위한 변수와 함수
	UPROPERTY(EditAnywhere, Category="Boss|Pateern")
	float patternInterval = 1.0f;
	void ChoosePattern();
	void EndPattern();
	EBossPattern CurrentPattern = EBossPattern::Idle;
	
	//십자 차징 두번하게
	int32 currentDoubleChargeCount = 0;
	UPROPERTY(EditAnywhere)
	int32 maxDoubleChargeCount = 2;
};
