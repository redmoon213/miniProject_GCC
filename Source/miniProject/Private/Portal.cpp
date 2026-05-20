// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"


// Sets default values
APortal::APortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(boxComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	meshComp->SetupAttachment(boxComp);
	decalComp = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	decalComp->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	decalComp->SetupAttachment(boxComp);
	
	
}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	if (decalComp)
	{
		decalComp->SetHiddenInGame(true);
	}
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortal::ShowPortal()
{
	decalComp->SetHiddenInGame(false);
}
