// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class MINIPROJECT_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//박스콜리전과 스태틱메시 컴포넌트
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* damageBoxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	
	// Category -> 에디터에 찾기 쉽게 "Camera"탭이라는 이름을 붙여줌.
	UPROPERTY(EditAnywhere, Category="Camera")
	class USpringArmComponent* springArmComp;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	class UCameraComponent* cameraComp;
	
	//이동 컴포넌트
	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* moveComp;
	
	
	//입력관련 변수들
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imcPlayerInput;
	UPROPERTY(EditAnywhere)
	class UInputAction* iaHorizontal;
	UPROPERTY(EditAnywhere)
	class UInputAction* iaVertical;
	UPROPERTY(EditAnywhere)
	class UInputAction* iaFire;
	UPROPERTY(EditAnywhere)	
	class UInputAction* iaDash;
	UPROPERTY(EditAnywhere)
	class UInputAction* iaChangeWeapon;

	UPROPERTY(EditAnywhere)
	class UInputAction* iaUseSkill_Q;

	UPROPERTY(EditAnywhere)
	class UInputAction* iaUseSkill_E;
	
	
	UPROPERTY(EditAnywhere)
	float movementSpeed = 1000.0f;
	
	// 플레이어 커서 위젯을 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerCursor> playerCursorClass;
	
	// 총구 컴포넌트 
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;
	
	// 무기별 탄환 블루프린트 배열
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TArray<TSubclassOf<class ABulletPlayerBasic>> bulletFactories;

	// 나선형 스킬 총알 클래스
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ABulletPlayerSpiral> spiralBulletClass;
	
	//발사 관련 변수
	
	int32 fireMode = 2;
	FTimerHandle autoFireTimerHandle;
	
	UPROPERTY(EditAnywhere)
	float autoFireRate = 0.1f;
	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound;
	
	UPROPERTY(EditAnywhere)
	class USoundBase* fireSoundBasic;
	
	/*UPROPERTY(EditAnywhere)
	class USoundBase* fireSound;
	*/
	
	
	int32 currentAmmo = 0;
	int32 maxAmmo = 30;

	// 탄퍼짐 관련 변수
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float minSpread = 0.0f;         // 기본 탄퍼짐 (도 단위)
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float maxSpread = 10.0f;          // 최대 탄퍼짐 (도 단위) - 범위를 늘림
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float spreadIncrement = 1.2f;    // 발당 증가량 (상향)
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float spreadRecoveryRate = 3.0f; // 초당 회복량 (하향)

	float currentSpread = 0.0f;      // 현재 누적된 탄퍼짐

	// 발사 간격 관련 변수
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float fireDelay = 0.2f; // 기본총 발사 간격 (초 단위)
	float lastFireTime = 0.0f;       // 마지막 발사 시간 기록
	
	//대쉬기능에 관련된 변수들
	UPROPERTY(EditAnywhere)
	class UTimelineComponent* dashTimelineComp;
	UPROPERTY(EditAnywhere)
	class UCurveFloat* dashCurve;
	
	
	FVector dashDirection;
	bool isDashing;
	float dashCurrentCoolTime = 0.0f;
	float dashMaxCoolTime = 2.0f;
	
	
	UFUNCTION()
	void DashProgress(float Value);
	UFUNCTION()
	void DashFinished();
	
	//플레이어가 피해를 입을때 사용할 변수와 함수
	FTimerHandle invincibleTimerHandle;
	
	UPROPERTY(EditAnywhere)
	int32 playerHp = 6;
	
	UPROPERTY(editAnywhere)
	float invincibleDuration = 1.0f;
	
	bool bIsInvincible = false;
	void ResetInvincibility();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY()
	class UMaterialInstanceDynamic* dynamicMaterial;
	
	// 플레이어 UI
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHealthUI> playerHealthUIClass;
	
	UPROPERTY(EditAnywhere)
	class UPlayerHealthUI* playerHealthUIInstance;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UPlayerMagUI> qSkillUiClass;
	UPROPERTY(EditAnywhere)
	class UPlayerMagUI* qSkillUiInstance;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UPlayerMagUI> eSkillUiClass;
	UPROPERTY(EditAnywhere)
	class UPlayerMagUI* eSkillUiInstance;
	
	//
	void LootAmmo(int32 ammoAmount);

	// 스킬 관련 함수 및 변수
	void UseSkill();
	void UseSkill_E();
	
	void ExecuteSkillWave();
	
	void SpawnFanBullets(int32 bulletCount, float spreadAngle);

	int32 skillWaveCount = 0;
	FTimerHandle skillTimerHandle;

	// E 스킬 쿨다운 관련
	UPROPERTY(EditAnywhere, Category = "Skill")
	float skillECoolDown = 5.0f;

	float currentSkillECoolDown = 0.0f;
	bool bCanUseSkillE = true;
	
private:
	//float h;
	//float v;
	
	class UPlayerCursor* playerCursorInstance;
	
	
	void OnInputVertical(const struct FInputActionValue& value);
	void OnInputHorizontal(const struct FInputActionValue& value);
	
	
	void StartFire();
	void Fire();
	void EndFire();
	
	void ChangeWeapon(const struct FInputActionValue& value);
	
	void Dash(const struct FInputActionValue& value);
};
