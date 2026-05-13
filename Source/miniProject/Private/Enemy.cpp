// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EngineUtils.h"
#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	SetRootComponent(boxComp);
	FVector boxSize = FVector(50.f, 50.f, 50.f);
	boxComp->SetBoxExtent(boxSize);
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
	
	
	
	// 데칼 컴포넌트 설정
	attackDecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Attack Decal Component"));
	attackDecalComp->SetupAttachment(boxComp);
	
	attackDecalComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	attackDecalComp->DecalSize = FVector(100.f, 100.f, 200.f);

	
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
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 넉백 구현 코드
	if (!knockbackSpeed.IsNearlyZero())
	{
		AddActorWorldOffset(knockbackSpeed * DeltaTime, true);
		
		knockbackSpeed = FMath::VInterpTo(knockbackSpeed, FVector::ZeroVector, DeltaTime, 5.0f);
	}
	
	// 이동 구현 코드
	//FVector newLocation = GetActorLocation() + dir * moveSpeed * DeltaTime;
	//SetActorLocation(newLocation, true);
	
	
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
