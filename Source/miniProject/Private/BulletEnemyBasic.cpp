// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletEnemyBasic.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABulletEnemyBasic::ABulletEnemyBasic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Enemy Bullet Box Component"));
	SetRootComponent(boxComp);
	
	FVector boxSize = FVector(50.f,50.0f,50.0f);
	boxComp->SetBoxExtent(boxSize);
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Enemy Bullet Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	
	boxComp->SetCollisionProfileName("EnemyBullet");
}

// Called when the game starts or when spawned
void ABulletEnemyBasic::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletEnemyBasic::OnOverlapToPlayer);
	
	player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

// Called every frame
void ABulletEnemyBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector dir = GetActorForwardVector();
	
	this->AddActorWorldOffset(dir * moveSpeed * DeltaTime, true);
	
}

void ABulletEnemyBasic::OnOverlapToPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor==player)
	{
		UGameplayStatics::ApplyDamage(player, 1.0f, nullptr, this, nullptr);
		Destroy();
	}
	// 벽 태그를 가진 액터와 충돌했는지 확인합니다.
	else if (OtherActor && OtherActor->ActorHasTag(TEXT("Wall")))
	{
		Destroy();
	}
}



