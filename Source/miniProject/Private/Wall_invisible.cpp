// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall_invisible.h"

#include "Components/BoxComponent.h"


// Sets default values
AWall_invisible::AWall_invisible()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(boxComp);
}

// Called when the game starts or when spawned
void AWall_invisible::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall_invisible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

