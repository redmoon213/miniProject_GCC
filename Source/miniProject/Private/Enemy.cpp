// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "BulletEnemyBasic.h"
#include "EngineUtils.h"
#include "PlayerPawn.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//박스 컴포넌트 설정
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	SetRootComponent(boxComp);
	FVector boxSize = FVector(50.f, 50.f, 50.f);
	boxComp->SetBoxExtent(boxSize);
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
	
	//메쉬 컴포넌트 설정
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	
	
	
	// 데칼 컴포넌트 설정
	attackDecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Attack Decal Component"));
	attackDecalComp->SetupAttachment(boxComp);
	
	attackDecalComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	attackDecalComp->DecalSize = FVector(100.f, 100.f, 200.f);
	
	// 투사체 총구 컴포넌트 설정
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position Component"));
	firePosition->SetupAttachment(boxComp);
	firePosition->SetRelativeLocation(FVector(25.f, 0.f, 0.f));
	

	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 주사위 눈금을 결정하고 업데이트
	UMaterialInterface* baseMaterial = meshComp->GetMaterial(0);
	if (baseMaterial!=nullptr)
	{
		dynamicMaterial = meshComp->CreateDynamicMaterialInstance(0, baseMaterial);
		
	}
	currentDiceEye = FMath::RandRange(1, 6);
	UpdateDiceEye();
	
	
	//생성시 플레이어 방향으로 이동하게 하는 코드
	for (TActorIterator<APlayerPawn> player(GetWorld()); player; ++player)
	{
		if (player->GetName().Contains(TEXT("BP_PlayerPawn")))
		{
			dir = player->GetActorLocation() - GetActorLocation();
			dir.Normalize();
		}
	}
	
	// 공격 데칼 설정
	if (attackDecalComp->GetDecalMaterial() != nullptr)
	{
		dynamicAttackMaterial = attackDecalComp->CreateDynamicMaterialInstance();
	}
	
	attackDecalComp->SetHiddenInGame(true);
	isCharging = false;
	maxChargeTime = 3.0f;
	////////
	
	//생성된 주사위가 어떤 행동을 할지 결정?
	attackMode = FMath::RandRange(1,3);
	
	if (attackMode <1 || attackMode>3 )
	{
		attackMode = 1;
	}
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 넉백 구현 코드
	if (!knockbackSpeed.IsNearlyZero())
	{
		AddActorWorldOffset(knockbackSpeed * DeltaTime, false);
		
		knockbackSpeed = FMath::VInterpTo(knockbackSpeed, FVector::ZeroVector, DeltaTime, 5.0f);
	}
	
	// 이동 구현 코드
	//FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;
	//SetActorLocation(newLocation, true);
	
	//근접공격 if attackMode = 1
	
	
	//투사체 공격 if attackMode = 2
	
	
	//차지 공격 if attackMode = 3
	
	
	// 차지공격 구현 코드
	if (isCharging && dynamicAttackMaterial)
	{
		currentChargeTime += DeltaTime;
		float chargePercent = FMath::Clamp(currentChargeTime/maxChargeTime, 0.f, 1.f);
		
		dynamicAttackMaterial->SetScalarParameterValue(TEXT("Percent"), chargePercent);
		
		if (chargePercent >= 1.0f)
		{
			isCharging = false;
			attackDecalComp->SetHiddenInGame(true);
			
			ChargingExcute();
		}
	}
}

void AEnemy::UpdateDiceEye()
{
	if (dynamicMaterial!= nullptr && currentDiceEye >=1)
	{
		FVector2D UVoffset = diceOffset[currentDiceEye-1];
		
		dynamicMaterial->SetScalarParameterValue("UOffset", UVoffset.X);
		dynamicMaterial->SetScalarParameterValue("VOffset", UVoffset.Y);
	}
}

void AEnemy::diceTakeDamage()
{
	if (currentDiceEye == 1)
	{
		this->Destroy();
	}
	
	else
	{
		dynamicMaterial->SetScalarParameterValue("HitFlash", 1.0f);
		currentDiceEye --;
		UpdateDiceEye();
	}
	
	GetWorld()->GetTimerManager().SetTimer(hitFlashTimer, this, &AEnemy::ResetHitFlash, 0.1f, false);
	
	Charging();
	
}

void AEnemy::ResetHitFlash()
{
	if (dynamicMaterial!=nullptr)
	{
		dynamicMaterial->SetScalarParameterValue("HitFlash", 0.0f);
	}
}

void AEnemy::Knockback(FVector bulletDirection)
{
	knockbackSpeed = bulletDirection * 500.0f;
}


void AEnemy::Charging()
{
	currentChargeTime = 0.0f;
	isCharging = true;
	attackDecalComp->SetHiddenInGame(false);
}

void AEnemy::ChargingExcute()
{
	UE_LOG(LogTemp, Warning, TEXT("Excute charge"));
	
}

void AEnemy::FireProjectile()
{
	ABulletEnemyBasic* enemyBullet = GetWorld()->SpawnActor<ABulletEnemyBasic>(bulletFactory,
		firePosition->GetComponentLocation(), firePosition->GetComponentRotation()
		);
	
	
}
