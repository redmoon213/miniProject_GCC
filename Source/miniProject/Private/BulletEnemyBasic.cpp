// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletEnemyBasic.h"

#include "Components/BoxComponent.h"


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
}

// Called when the game starts or when spawned
void ABulletEnemyBasic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletEnemyBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector dir = GetActorForwardVector();
	
	this->AddActorWorldOffset(dir * moveSpeed * DeltaTime, true);
	
}

