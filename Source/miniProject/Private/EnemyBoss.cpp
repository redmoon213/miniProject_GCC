// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"

#include "BulletEnemyBasic.h"
#include "MyPlayerController.h"
#include "EnemyBossHpUI.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h" // 추가: 판정 범위 시각화를 위해 필요
#include "Components/ArrowComponent.h"
#include "Engine/OverlapResult.h"


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
	decalSouth->SetRelativeRotation(FRotator(-90.0f, 180.0f, 0.0f));

	decalEast = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalEast"));
	decalEast->SetupAttachment(decalGroup);
	decalEast->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	decalWest = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalWest"));
	decalWest->SetupAttachment(decalGroup);
	decalWest->SetRelativeRotation(FRotator(-90.0f, 0.0f, 180.0f));

	// 기본값 설정 (에디터에서 수정 가능)
	maxDecalLength = 1000.0f;
	decalWidth = 100.0f;

	// 생성자에서 초기 위치를 잡아주지만, 실제 조절은 에디터에서 수행 권장
	decalNorth->DecalSize = FVector(500.0f, decalWidth, maxDecalLength);
	decalNorth->SetRelativeLocation(FVector(maxDecalLength / 2.0f, 0.0f, 0.0f));

	decalSouth->DecalSize = FVector(500.0f, decalWidth, maxDecalLength);
	decalSouth->SetRelativeLocation(FVector(-maxDecalLength / 2.0f, 0.0f, 0.0f));

	decalEast->DecalSize = FVector(500.0f, maxDecalLength, decalWidth);
	decalEast->SetRelativeLocation(FVector(0.0f, maxDecalLength / 2.0f, 0.0f));

	decalWest->DecalSize = FVector(500.0f, maxDecalLength, decalWidth);
	decalWest->SetRelativeLocation(FVector(0.0f, -maxDecalLength / 2.0f, 0.0f));
	
	//투사체 발사를 위한 변수 초기화
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	firePosition->SetupAttachment(boxComp);
	
	boxComp->SetCollisionProfileName("Enemy");
	maxHp = 100.0f;
	currentHp = maxHp;

	// 각 패턴별 기본 후딜레이 초기화 (1.0초)
	PatternPostDelays.Add(EBossPattern::Projectile, 1.0f);
	PatternPostDelays.Add(EBossPattern::Charging, 1.0f);
	PatternPostDelays.Add(EBossPattern::JumpAttack, 1.0f);
	PatternPostDelays.Add(EBossPattern::Moving, 1.0f);
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
	
	// 플레이어 컨트롤러를 통해 보스 체력 바 표시 및 도입부 시작
	StartIntro();
}

void AEnemyBoss::StartIntro()
{
	CurrentPattern = EBossPattern::Intro;
	bCanRotate = false;

	float finalIntroTime = 2.5f; // 기본값

	AMyPlayerController* pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		pc->ShowBossHPBar(TEXT("DICE BOSS"));
		pc->UpdateBossHP(currentHp, maxHp);

		if (pc->bossHpUIInstance)
		{
			// UI에 설정된 차오르는 시간을 가져와서 보스 대기 시간으로 사용
			finalIntroTime = pc->bossHpUIInstance->GetFillDuration();
		}
	}

	// UI 애니메이션 시간만큼 대기 후 도입부 종료 함수 호출
	GetWorldTimerManager().SetTimer(bossPatternHandle, this, &AEnemyBoss::FinishIntro, finalIntroTime, false);
}

void AEnemyBoss::FinishIntro()
{
	CurrentPattern = EBossPattern::Idle;
	bCanRotate = true;
	
	// 첫 번째 패턴 선택
	ChoosePattern();
}

// Called every frame
void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCharging)
	{
		UpdateCrossCharge(DeltaTime);
	}
	
	if (bIsMoving)
	{
		MoveForward(DeltaTime);
	}
	
	if (bCanRotate)
	{
		SearchPlayer(DeltaTime);
	}
	
	if (bIsShaking)
	{
		UpdateShaking(DeltaTime);
	}
	
	if (bIsJumping)
	{
		
		UpdateJumpAttack(DeltaTime);
	}
	
	
}

void AEnemyBoss::StartCrossCharge()
{
	currentChargeTime = 0.0f;
	bIsCharging = true;
	bCanRotate = false;
	currentDoubleChargeCount = 0;
	
	//currentRadius += nextRadius;
	//decalGroup->SetRelativeRotation(FRotator(0.0f, currentRadius, 0.0f));
	
	decalGroup->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	
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
			
			
			FVector decalSize = decal->DecalSize;
			FVector scale = decal->GetComponentScale();
			FVector boxExtent;
			boxExtent.X = (decalSize.X * scale.X) / 2.0f;
			boxExtent.Y = (decalSize.Y * scale.Y) / 2.0f;
			boxExtent.Z = (decalSize.Z * scale.Z) / 2.0f;
			
			TArray<AActor*> overlappedActors;
			
			// --- 전용 트레이스 채널(EnemyChargeAttack)을 이용한 판정 로직 ---
			// 에디터에서 첫 번째로 추가한 트레이스 채널은 보통 ECC_GameTraceChannel1에 해당합니다.
			TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
			TArray<FOverlapResult> overlapResults;
			FCollisionObjectQueryParams objectParams;
			objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
			
			bool bHit = GetWorld()->OverlapMultiByObjectType(
				overlapResults,
				overlapLocation,
				overlapRotation.Quaternion(),
				objectParams,
				FCollisionShape::MakeBox(boxExtent)
				);
			

			/*// 전용 채널 필터를 적용하여 정확한 오버랩 체크 수행
			UKismetSystemLibrary::BoxOverlapActors(
				GetWorld(),
				overlapLocation,
				boxExtent,
				objectTypes,
				nullptr,
				ignoreActors,
				overlappedActors
			);*/
			
			
			/*
			for (AActor* actor : overlappedActors)
			{
				totalHitActors.AddUnique(actor);
			}*/
			for (const FOverlapResult& result : overlapResults)
			{
				AActor* hitActor = result.GetActor();
				if (hitActor && hitActor != this)
				{
					totalHitActors.AddUnique(hitActor);
				}
			}
			
			//DrawDebugBox(GetWorld(), overlapLocation, boxExtent, overlapRotation.Quaternion(), FColor::Red, false, 1.0f, 0, 5.0f);

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
	
	currentDoubleChargeCount++;
	if (currentDoubleChargeCount < maxDoubleChargeCount)
	{
		currentChargeTime = 0.0f;
		bIsCharging = true;
		decalGroup->SetRelativeRotation(FRotator(0.0f, 45.0f, 0.0f));
		if (decalNorth) decalNorth->SetHiddenInGame(false);
		if (decalSouth) decalSouth->SetHiddenInGame(false);
		if (decalEast) decalEast->SetHiddenInGame(false);
		if (decalWest) decalWest->SetHiddenInGame(false);
	}
	else
	{
		EndPattern();
	}
}


void AEnemyBoss::StartProjectileBossPattern()
{
	bCanRotate = false;
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
	currentProjectileRadius = 0.0f;
	EndPattern();
}

void AEnemyBoss::MoveStart()
{
	if (!bIsMoving)
	{
		bIsMoving = true;
		hopAlpha = 0.0f;
		//bCanRotate = false;
	}
}

void AEnemyBoss::MoveForward(float DeltaTime)
{
	
	hopAlpha += DeltaTime / hopDuration;
	
	float sinValue = FMath::Sin(hopAlpha*PI);
	float currentHop = sinValue * hopHeight;
	
	FVector newLocation = GetActorLocation() + (GetActorForwardVector() * moveSpeed * DeltaTime);
	newLocation.Z = baseZ + currentHop;
	SetActorLocation(newLocation);
	
	if (hopAlpha >= 1.0f)
	{
		FVector finalloc = GetActorLocation();
		finalloc.Z = baseZ;
		SetActorLocation(finalloc);
		
		EndPattern();
	}
	
}

void AEnemyBoss::SearchPlayer(float DeltaTime)
{
	
	FVector dir = player->GetActorLocation() - GetActorLocation();
	dir.Z = 0.0f;
	dir.Normalize();
	FRotator targetRotation = dir.Rotation();
	
	FRotator smoothRotation = FMath::RInterpTo(GetActorRotation(), targetRotation, DeltaTime, 5.0f);
	
	SetActorRotation(smoothRotation);

}

float AEnemyBoss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser&&DamageCauser->ActorHasTag((FName("Debris"))))
	{
		return 0.0f;
	}

	// 피격 사운드 재생
	if (hitSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, hitSound, GetActorLocation());
	}
	
	currentHp -= FMath::RoundToInt(DamageAmount);

	// UI 업데이트
	AMyPlayerController* pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		pc->UpdateBossHP(currentHp, maxHp);
	}
	
	hitFlashDynamicMaterial->SetScalarParameterValue("HitFlash", 1.0f);
	GetWorld()->GetTimerManager().SetTimer(hitFlashHandle, this, &AEnemyBoss::ResetHitFlash, 0.1f, false);
	
	if (currentHp <= 0)
	{
		if (pc)
		{
			pc->HideBossHPBar();
		}
		Destroy();
	}
	
	return DamageAmount;
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


void AEnemyBoss::StartJumpAttack()
{
	bIsShaking = true;
	currentShakeTime = 0.0f;
	
	bCanRotate = false;
	
	return;
}

void AEnemyBoss::ExecuteJumpAttack()
{
	if (!player)
	{
		return ;
	}
	bCanRotate = false;
	bIsJumping = true;
	jumpAlpha = 0.0f;
	jumpStartLocation = GetActorLocation();
	jumpTargetLocation = player->GetActorLocation();
	jumpTargetLocation.Z = baseZ;
	return;
}

void AEnemyBoss::UpdateJumpAttack(float DeltaTime)
{
	jumpAlpha += DeltaTime / jumpDurtation;
		
	FVector currentXY = FMath::Lerp(jumpStartLocation, jumpTargetLocation, jumpAlpha);
		
	float sinValue = FMath::Sin(jumpAlpha * PI);
	float currentZOffset = sinValue*jumpheigth;
	FVector newLocation = currentXY;
	newLocation.Z += currentZOffset;
	SetActorLocation(newLocation);
		
	if (jumpAlpha >= 1.0f)
	{
		
		SetActorLocation(jumpTargetLocation);
		
		EndPattern();
	}
}

void AEnemyBoss::UpdateShaking(float DeltaTime)
{
	currentShakeTime += DeltaTime;
		
	float sineValue = FMath::Sin(GetWorld()->GetTimeSeconds() * shakeSpeed);
	meshComp->SetRelativeRotation(FRotator(0, sineValue*shakeIntensity, 0));
		
	if (currentShakeTime >= shakeDuration)
	{
		bIsShaking = false;
		meshComp->SetRelativeRotation(FRotator::ZeroRotator);
			
		ExecuteJumpAttack();
	}
}

void AEnemyBoss::ChoosePattern()
{
	if (CurrentPattern != EBossPattern::Idle)
	{
		return;
	}
	int32 randomIdx = FMath::RandRange(2, static_cast<int32>(EBossPattern::MAX) - 1);
	
	EBossPattern selectedPattern = static_cast<EBossPattern>(randomIdx);
	
	switch (selectedPattern)
	{
	case EBossPattern::Projectile:
		StartProjectileBossPattern();
		break;
	case EBossPattern::Charging:
		StartCrossCharge();
		break;
	case EBossPattern::JumpAttack:
		StartJumpAttack();
		break;
	case EBossPattern::Moving:
		MoveStart();
		break;
	default:
		break;
	}
	
	CurrentPattern = selectedPattern;
}


void AEnemyBoss::EndPattern()
{
	// 1. 현재 종료되는 패턴의 후딜레이 값을 TMap에서 찾음
	float nextDelay = patternInterval; // 기본 fallback 값
	if (PatternPostDelays.Contains(CurrentPattern))
	{
		nextDelay = PatternPostDelays[CurrentPattern];
	}

	// 2. 상태 초기화
	CurrentPattern = EBossPattern::Idle;
	bIsShaking = false;
	bIsCharging = false;
	bIsJumping = false;
	bIsMoving = false;
	bIsProjectilePattern = false;
	bCanRotate = true;
	
	// 3. 결정된 지연 시간 후 다음 패턴 실행
	GetWorldTimerManager().SetTimer(bossPatternHandle, this, &AEnemyBoss::ChoosePattern, nextDelay, false);
}
