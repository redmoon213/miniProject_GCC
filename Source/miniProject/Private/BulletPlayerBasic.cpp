// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPlayerBasic.h"

#include "Components/BoxComponent.h"


// Sets default values
ABulletPlayerBasic::ABulletPlayerBasic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	SetRootComponent(boxComp);
	FVector boxSize = FVector(10.f, 10.f, 10.f);
	boxComp->SetBoxExtent(boxSize);
	boxComp->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My StaticMesh Component"));
	meshComp->SetupAttachment(boxComp);
	
	
}

// Called when the game starts or when spawned
void ABulletPlayerBasic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletPlayerBasic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

