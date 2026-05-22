// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPlayerSpiral.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABulletPlayerSpiral::ABulletPlayerSpiral()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(boxComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(boxComp);

	boxComp->SetCollisionProfileName(TEXT("PlayerBullet"));
}

// Called when the game starts or when spawned
void ABulletPlayerSpiral::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어 위치를 중심점으로 잡되, 묻힘 방지를 위해 Z값을 약간 올립니다.
	centerLocation = GetActorLocation();
	centerLocation.Z += zOffset;

	// 초기 각도 설정
	currentAngle = 0.0f;
	currentRadius = 0.0f;

	// 충돌 이벤트 연결
	if (boxComp)
	{
		boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletPlayerSpiral::OnOverlap);
	}
}

void ABulletPlayerSpiral::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 벽 태그 확인: 벽에 부딪혀도 무시하고 지나갑니다.
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Wall")))
	{
		return;
	}

	// 2. 적 또는 보스인지 확인
	// AEnemy 또는 AEnemyBoss 클래스인지 체크합니다.
	if (OtherActor && (OtherActor->IsA(AActor::StaticClass()))) // 기본적으로 모든 액터에 데미지 시도
	{
		// 플레이어 본인은 제외
		if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			return;
		}

		// 데미지 1 적용
		UGameplayStatics::ApplyDamage(OtherActor, 1.0f, nullptr, this, nullptr);
		
		// 적에게 맞았을 때는 소멸합니다. (관통을 원하시면 이 줄을 주석 처리 하세요)
		Destroy();
	}
}

// Called every frame
void ABulletPlayerSpiral::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 각도와 반지름을 프레임마다 증가시킵니다.
	currentAngle += rotationSpeed * DeltaTime;
	currentRadius += expansionSpeed * DeltaTime;

	// 나선형 위치 계산
	FVector newOffset;
	newOffset.X = FMath::Cos(currentAngle) * currentRadius;
	newOffset.Y = FMath::Sin(currentAngle) * currentRadius;
	newOffset.Z = 0.0f;

	SetActorLocation(centerLocation + newOffset);

	// 일정 거리 이상 나가면 파괴 (최적화)
	if (currentRadius > 3000.0f)
	{
		Destroy();
	}
}

