// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "BulletEnemyBasic.h"
#include "EnemyIndicatorWidget.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"


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
	
	player = UGameplayStatics::GetPlayerPawn(this, 0);
	
	// 공격 데칼 설정
	if (attackDecalComp->GetDecalMaterial() != nullptr)
	{
		dynamicAttackMaterial = attackDecalComp->CreateDynamicMaterialInstance();
	}
	
	attackDecalComp->SetHiddenInGame(true);
	
	
	isCharging = false;
	maxChargeTime = 1.5f;
	
	////////
	
	//생성된 주사위가 어떤 행동을 할지 결정?
	//attackMode = FMath::RandRange(1,3);
	attackMode = 1;
	
	if (attackMode <1 || attackMode>3 )
	{
		attackMode = 1;
	}
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapToPlayer);
	
	indicatorInstance = CreateWidget<UEnemyIndicatorWidget>(GetWorld(), indicatorWidget);
	if (indicatorInstance != nullptr)
	{
		indicatorInstance->SetTarget(this);
		
		indicatorInstance->AddToViewport();
		
	}
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsValid(player))
	{
		return;
	}
	bool isBeingKnockback = false;
	// 넉백 구현 코드
	if (!knockbackSpeed.IsNearlyZero(100.0f))
	{
		AddActorWorldOffset(knockbackSpeed * DeltaTime, true);
		
		knockbackSpeed = FMath::VInterpTo(knockbackSpeed, FVector::ZeroVector, DeltaTime, 5.0f);
		isBeingKnockback = true;
	}
	
	if (!isBeingKnockback)
	{
		switch (attackMode)
		{
		case 1: MoveToPlayer(DeltaTime);
			break;
		case 2: FireProjectileMode(DeltaTime);
			break;
		case 3: ChargingMode(DeltaTime);
			break;
		default:
			MoveToPlayer(DeltaTime);
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
	if (isCharging)
	{
		return;
	}
	knockbackSpeed = bulletDirection * 1000.0f;
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

void AEnemy::MoveToPlayer(float deltaTime)
{
	
	
	FVector direction = player->GetActorLocation() - GetActorLocation();
	direction.Z = 0.0f;
	direction.Normalize();
	
	FRotator targetRotation = direction.Rotation();
	
	FRotator currentRotation = GetActorRotation();
	FRotator smoothRotation = FMath::RInterpTo(currentRotation, targetRotation, deltaTime, 5.0f);
	
	AddActorWorldOffset(direction * deltaTime * moveSpeed, true);
	SetActorRotation(smoothRotation);
}

void AEnemy::FireProjectileMode(float deltaTime)
{
	
	
	
	FVector dirToPlayer = player->GetActorLocation() - GetActorLocation();
	FVector enemyLocation = GetActorLocation();
	
	float distance = dirToPlayer.Size();
	dirToPlayer.Normalize();
	
	SetActorRotation(dirToPlayer.Rotation());
	FVector moveDirection = FVector::ZeroVector;
	
	float minDistance = 400.0f;
	float maxDistance = 800.0f;
	
	if (distance > maxDistance)
	{
		moveDirection += dirToPlayer;
	}
	
	else if (distance < minDistance)
	{
		moveDirection -= dirToPlayer;
	}
	
	FVector rightVector = GetActorRightVector();
	float wiggleSpeed = 2.0f;
	float wiggleIntensity = 1.0f;
	
	float sineValue = FMath::Sin(GetWorld()->GetTimeSeconds() * wiggleSpeed);
	moveDirection += rightVector * sineValue * wiggleIntensity;
	
	if (!moveDirection.IsNearlyZero())
	{
		moveDirection.Normalize();
		AddActorWorldOffset(moveDirection * moveSpeed * deltaTime, true);
	}
	
	
	currentProjectileCooldown += deltaTime;
	
	if (currentProjectileCooldown>=maxProjectileCooldown)
	{
		ABulletEnemyBasic* enemyBullet = GetWorld()->SpawnActor<ABulletEnemyBasic>(bulletFactory,
			firePosition->GetComponentLocation(), firePosition->GetComponentRotation()
		);
		
		currentProjectileCooldown = 0.0f;
	}
}

void AEnemy::ChargingMode(float deltaTime)
{
	currentChargeCoolTime += deltaTime;
	
	if (currentChargeCoolTime >= 3.0f)
	{
		Charging();
		currentChargeCoolTime = 0.0f;
	}
	
	if (!isCharging)
	{
		
	
		FVector dirToPlayer = player->GetActorLocation() - GetActorLocation();
		FVector enemyLocation = GetActorLocation();
	
		float distance = dirToPlayer.Size();
		dirToPlayer.Normalize();
	
		SetActorRotation(dirToPlayer.Rotation());
		FVector moveDirection = FVector::ZeroVector;
	
		float minDistance = 400.0f;
		float maxDistance = 800.0f;
	
		if (distance > maxDistance)
		{
			moveDirection += dirToPlayer;
		}
	
		else if (distance < minDistance)
		{
			moveDirection -= dirToPlayer;
		}
	
		FVector rightVector = GetActorRightVector();
		float wiggleSpeed = 2.0f;
		float wiggleIntensity = 1.0f;
	
		float sineValue = FMath::Sin(GetWorld()->GetTimeSeconds() * wiggleSpeed);
		moveDirection += rightVector * sineValue * wiggleIntensity;
	
		if (!moveDirection.IsNearlyZero())
		{
			moveDirection.Normalize();
			AddActorWorldOffset(moveDirection * moveSpeed * deltaTime, true);
		}
	}
	if (isCharging && dynamicAttackMaterial)
	{
		currentChargeTime += deltaTime;
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

void AEnemy::OnOverlapToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == player)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 1.0f, nullptr, this, nullptr);
	}
}
