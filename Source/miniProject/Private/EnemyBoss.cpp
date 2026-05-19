// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"

#include "BulletEnemyBasic.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h" // 추가: 판정 범위 시각화를 위해 필요
#include "Components/ArrowComponent.h"


// Sets default values
AEnemyBoss::AEnemyBoss()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(boxComp);
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	meshComp->SetupAttachment(boxComp);

	// 데칼들을 한꺼번에 관리하기 위한 상위 컴포넌트 생성
	decalGroup = CreateDefaultSubobject<USceneComponent>(TEXT("DecalGroup"));
	decalGroup->SetupAttachment(boxComp);

	// --- 4방향 차징 공격 데칼 설정 ---

	decalNorth = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalNorth"));
	decalNorth->SetupAttachment(decalGroup);
	decalNorth->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	decalSouth = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalSouth"));
	decalSouth->SetupAttachment(decalGroup);
	decalSouth->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	decalEast = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalEast"));
	decalEast->SetupAttachment(decalGroup);
	decalEast->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	decalWest = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalWest"));
	decalWest->SetupAttachment(decalGroup);
	decalWest->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	// 기본값 설정 (에디터에서 수정 가능)
	maxDecalLength = 1000.0f;
	decalWidth = 100.0f;

	// 생성자에서 초기 위치를 잡아주지만, 실제 조절은 에디터에서 수행 권장
	decalNorth->DecalSize = FVector(500.0f, decalWidth, maxDecalLength);
	decalNorth->SetRelativeLocation(FVector(maxDecalLength / 2.0f, 0.0f, 0.0f));

	decalSouth->DecalSize = FVector(500.0f, decalWidth, maxDecalLength);
	decalSouth->SetRelativeLocation(FVector(-maxDecalLength / 2.0f, 0.0f, 0.0f));
	decalSouth->SetRelativeRotation(FRotator(-90.0f, 180.0f, 0.0f));

	decalEast->DecalSize = FVector(500.0f, maxDecalLength, decalWidth);
	decalEast->SetRelativeLocation(FVector(0.0f, maxDecalLength / 2.0f, 0.0f));

	decalWest->DecalSize = FVector(500.0f, maxDecalLength, decalWidth);
	decalWest->SetRelativeLocation(FVector(0.0f, -maxDecalLength / 2.0f, 0.0f));
	decalWest->SetRelativeRotation(FRotator(-90.0f, 0.0f, 180.0f));
	
	//투사체 발사를 위한 변수 초기화
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	firePosition->SetupAttachment(boxComp);
	
	boxComp->SetCollisionProfileName("Enemy");
	maxHp = 100.0f;
	currentHp = maxHp;
}

// Called when the game starts or when spawned
void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss::OnOverlapToPlayer);
	
	// 다이내믹 머티리얼 인스턴스 생성 및 초기화
	auto CreateMaterial = [this](UDecalComponent* Decal, UMaterialInstanceDynamic*& Mat) {
		if (Decal && Decal->GetDecalMaterial()) {
			Mat = Decal->CreateDynamicMaterialInstance();
			Decal->SetHiddenInGame(true);
		}
	};
	
	UMaterialInterface* baseMaterial = meshComp->GetMaterial(0);
	if (baseMaterial)
	{
		hitFlashDynamicMaterial = meshComp->CreateDynamicMaterialInstance(0, baseMaterial);
	}

	CreateMaterial(decalNorth, matNorth);
	CreateMaterial(decalSouth, matSouth);
	CreateMaterial(decalEast, matEast);
	CreateMaterial(decalWest, matWest);

	//
	baseZ = GetActorLocation().Z;
	//// 테스트용: 시작 시 차징 시작
	GetWorld()->GetTimerManager().SetTimer(testHandle, this ,&AEnemyBoss::StartCrossCharge, 2.0f, true);
	
	StartProjectileBossPattern();
	MoveStart();
	
	

}

// Called every frame
void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCharging)
	{
		UpdateCrossCharge(DeltaTime);
	}
	
	MoveStart();
	SearchPlayer(DeltaTime);
	MoveForward(DeltaTime);
}

void AEnemyBoss::StartCrossCharge()
{
	currentChargeTime = 0.0f;
	bIsCharging = true;
	bCanRotate = false;
	
	currentRadius += nextRadius;
	decalGroup->SetRelativeRotation(FRotator(0.0f, currentRadius, 0.0f));
	
	/*if (bChargeDirectionMode)
	{
	decalGroup->SetRelativeRotation(FRotator(0.0f, -45.0f, 0.0f));
		bChargeDirectionMode = false;
	}
	else
	{
		decalGroup->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		bChargeDirectionMode = true;
	}*/
	
	if (decalNorth) decalNorth->SetHiddenInGame(false);
	if (decalSouth) decalSouth->SetHiddenInGame(false);
	if (decalEast) decalEast->SetHiddenInGame(false);
	if (decalWest) decalWest->SetHiddenInGame(false);

	UE_LOG(LogTemp, Warning, TEXT("보스: 십자 차징 공격 시작!"));
}

void AEnemyBoss::UpdateCrossCharge(float DeltaTime)
{
	currentChargeTime += DeltaTime;
	float chargePercent = FMath::Clamp(currentChargeTime / maxChargeTime, 0.0f, 1.0f);

	auto UpdateMat = [&](UMaterialInstanceDynamic* Mat) {
		if (Mat) Mat->SetScalarParameterValue(TEXT("Percent"), chargePercent);
	};
	UpdateMat(matNorth);
	UpdateMat(matSouth);
	UpdateMat(matEast);
	UpdateMat(matWest);

	if (chargePercent >= 1.0f)
	{
		ExecuteCrossCharge();
	}
}

void AEnemyBoss::ExecuteCrossCharge()
{
	bIsCharging = false;

	// 데칼 숨김 처리
	if (decalNorth) decalNorth->SetHiddenInGame(true);
	if (decalSouth) decalSouth->SetHiddenInGame(true);
	if (decalEast) decalEast->SetHiddenInGame(true);
	if (decalWest) decalWest->SetHiddenInGame(true);

	// --- 에디터 동기화 공격 판정 로직 ---

	UDecalComponent* attackDecals[] = { decalNorth, decalSouth, decalEast, decalWest };
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);
	TArray<AActor*> totalHitActors;

	for (UDecalComponent* decal : attackDecals)
	{
		if (decal)
		{
			// 에디터에서 설정된 트랜스폼 및 크기 획득
			FVector overlapLocation = decal->GetComponentLocation();
			FRotator overlapRotation = decal->GetComponentRotation();
			
			// DecalSize를 기반으로 판정 범위 계산 (Extent는 절반값)
			FVector boxExtent = (decal->DecalSize * decal->GetComponentScale()) / 2.0f;

			TArray<AActor*> overlappedActors;
			
			// --- 전용 트레이스 채널(EnemyChargeAttack)을 이용한 판정 로직 ---
			// 에디터에서 첫 번째로 추가한 트레이스 채널은 보통 ECC_GameTraceChannel1에 해당합니다.
			TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
			objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

			

			// 전용 채널 필터를 적용하여 정확한 오버랩 체크 수행
			UKismetSystemLibrary::BoxOverlapActors(
				GetWorld(),
				overlapLocation,
				boxExtent,
				objectTypes,
				nullptr,
				ignoreActors,
				overlappedActors
			);
			
			

			for (AActor* actor : overlappedActors)
			{
				totalHitActors.AddUnique(actor);
			}
		}
	}	

	// 데미지 적용
	for (AActor* hitActor : totalHitActors)
	{
		if (hitActor && hitActor == player)
		{
			
			UGameplayStatics::ApplyDamage(hitActor, 1.0f, nullptr, this, nullptr);
			UE_LOG(LogTemp, Warning, TEXT("보스: 십자 공격 플레이어 적중!"));
		}
	}

	bCanRotate = true;
}


void AEnemyBoss::StartProjectileBossPattern()
{
	
	bIsProjectilePattern = true;
	GetWorld()->GetTimerManager().SetTimer(projectileHandle, this, &AEnemyBoss::FireProjectile, 0.1f, true);
}

void AEnemyBoss::FireProjectile()
{
	
	
	if (bIsProjectilePattern)
	{
		if (currentProjectileRadius >= maxProjectileRadius)
		{
			EndProjectilePattern();
		}
		
		
		FRotator fowardDir = firePosition->GetComponentRotation() + FRotator(0.0f, currentProjectileRadius, 0.0f);
		FRotator backwardDir = firePosition->GetComponentRotation() - FRotator(0.0f, 180.0f -currentProjectileRadius , 0.0f);
		bossBulletInstance = GetWorld()->SpawnActor<ABulletEnemyBasic>(bossBulletClass, firePosition->GetComponentLocation() + FVector(0.0f, 0.0f, -10.0f),
			fowardDir);
	
	
		bossBulletInstance = GetWorld()->SpawnActor<ABulletEnemyBasic>(bossBulletClass, firePosition->GetComponentLocation()+ FVector(0.0f, 0.0f, -10.0f),
			backwardDir);
		
		currentProjectileRadius += increaseProjectileRadius;
	}	
	
}

void AEnemyBoss::EndProjectilePattern()
{
	GetWorld()->GetTimerManager().ClearTimer(projectileHandle);
	bIsProjectilePattern = false;
	currentProjectileRadius = 0.0f;
}

void AEnemyBoss::MoveStart()
{
	if (!bIsMoving)
	{
		bIsMoving = true;
		hopAlpha = 0.0f;
		bCanRotate = false;
	}
}

void AEnemyBoss::MoveForward(float DeltaTime)
{
	if (!bIsMoving)
	{
		return;
	}
	
	hopAlpha += DeltaTime / hopDuration;
	
	float sinValue = FMath::Sin(hopAlpha*PI);
	float currentHop = sinValue * hopHeight;
	
	FVector newLocation = GetActorLocation() + (GetActorForwardVector() * moveSpeed * DeltaTime);
	newLocation.Z = baseZ + currentHop;
	SetActorLocation(newLocation);
	
	if (hopAlpha >= 1.0f)
	{
		bIsMoving = false;
		FVector finalloc = GetActorLocation();
		finalloc.Z = baseZ;
		SetActorLocation(finalloc);
		bCanRotate = true;
	}
	
}

void AEnemyBoss::SearchPlayer(float DeltaTime)
{
	/*if (!bCanRotate)
	{
		return;
	}*/
	FVector dir = player->GetActorLocation() - GetActorLocation();
	dir.Z = 0.0f;
	dir.Normalize();
	FRotator targetRotation = dir.Rotation();
	
	FRotator smoothRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, DeltaTime, 5.0f);
	
	SetActorRotation(smoothRotation);

}

float AEnemyBoss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	currentHp -= FMath::RoundToInt(DamageAmount);
	
	hitFlashDynamicMaterial->SetScalarParameterValue("HitFlash", 1.0f);
	GetWorld()->GetTimerManager().SetTimer(hitFlashHandle, this, &AEnemyBoss::ResetHitFlash, 0.1f, false);
	
	if (currentHp <= 0)
	{
		Destroy();
	}
	
	return 0.0f;
}

void AEnemyBoss::ResetHitFlash()
{
	hitFlashDynamicMaterial->SetScalarParameterValue("HitFlash", 0.0f);
}

void AEnemyBoss::OnOverlapToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == player)
	{
		UGameplayStatics::ApplyDamage(player, 1.0f, nullptr, this, nullptr);
	}
}
