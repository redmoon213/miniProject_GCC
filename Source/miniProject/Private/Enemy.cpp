// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/BoxComponent.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	SetRootComponent(boxComp);
	FVector boxSize = FVector(50.f, 50.f, 50.f);
	boxComp->SetBoxExtent(boxSize);
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Mesh Component"));
	meshComp->SetupAttachment(boxComp);
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterialInterface* baseMaterial = meshComp->GetMaterial(0);
	if (baseMaterial!=nullptr)
	{
		dynamicMaterial = meshComp->CreateDynamicMaterialInstance(0, baseMaterial);
		
	}
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	testTimeStack += DeltaTime;
	
	if (testTimeStack > 2.f)
	{
		currentDiceEye--;
		UpdateDiceEye();
		testTimeStack = 0;
	}
}

void AEnemy::UpdateDiceEye()
{
	if (dynamicMaterial!= nullptr && currentDiceEye >=1)
	{
		FVector2D UVindex = diceOffset[currentDiceEye-1];
		
		dynamicMaterial->SetScalarParameterValue("UOffset", UVindex.X);
		dynamicMaterial->SetScalarParameterValue("VOffset", UVindex.Y);
	}
}
