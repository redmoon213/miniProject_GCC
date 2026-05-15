// Fill out your copyright notice in the Description page of Project Settings.


#include "DroppedAmmo.h"

#include "Components/BoxComponent.h"


// Sets default values
ADroppedAmmo::ADroppedAmmo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(boxComp);
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(boxComp);
	
	ammoAmount = 10;
	
}

// Called when the game starts or when spawned
void ADroppedAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADroppedAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

