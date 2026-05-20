// Fill out your copyright notice in the Description page of Project Settings.


#include "WallVisible.h"

#include "Components/BoxComponent.h"


// Sets default values
AWallVisible::AWallVisible()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	meshComp->SetupAttachment(boxComp);
	
}

// Called when the game starts or when spawned
void AWallVisible::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallVisible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

