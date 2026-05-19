// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"

#include "Components/BoxComponent.h"


// Sets default values
AKillZone::AKillZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));


}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

