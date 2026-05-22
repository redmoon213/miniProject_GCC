// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPlayerBasic.h"

#include "Enemy.h"
#include "EnemyBoss.h"
#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABulletPlayerBasic::ABulletPlayerBasic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	SetRootComponent(boxComp);
	FVector boxSize = FVector(10.f, 10.f, 10.f);
	boxComp->SetBoxExtent(boxSize);
	FVector scaleVector = FVector (0.8f, 0.8f,0.8f);
	
	boxComp->SetWorldScale3D(scaleVector);
	
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My StaticMesh Component"));
	meshComp->SetupAttachment(boxComp);
	
	meshComp->SetWorldScale3D(scaleVector);
	
	boxComp->SetCollisionProfileName(TEXT("PlayerBullet"));
}

// Called when the game starts or when spawned
void ABulletPlayerBasic::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletPlayerBasic::OnBulletOverlap);
	
}

// Called every frame
void ABulletPlayerBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector newLocation = GetActorLocation() + GetActorForwardVector() * DeltaTime * moveSpeed;
	SetActorLocation(newLocation);
	
}



void ABulletPlayerBasic::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* enemyActor = Cast<AEnemy>(OtherActor);
	
	/*if (enemyActor!= nullptr)
	{
		enemyActor->diceTakeDamage();
		FVector bulletDirection = GetActorForwardVector();
		enemyActor->Knockback(bulletDirection);
		this->Destroy();
	}
	*/

	// 벽 태그를 가진 액터와 충돌했는지 확인합니다.
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Wall")))
	{
		Destroy();
		return;
	}

	// 보스 타격 시 탄약 획득 로직 추가
	if (AEnemyBoss* bossActor = Cast<AEnemyBoss>(OtherActor))
	{
		if (APlayerPawn* playerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
		{
			playerPawn->LootAmmo(1);
		}
	}
	
	UGameplayStatics::ApplyDamage(OtherActor, 1.0f, nullptr, this, nullptr);
	Destroy();
}