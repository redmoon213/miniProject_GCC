// Fill out your copyright notice in the Description page of Project Settings.


#include "DroppedAmmo.h"

#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


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
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ADroppedAmmo::OnOverlapBegin);
}

// Called every frame
void ADroppedAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADroppedAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("<TEST> droppedAmmo"));
	
	APlayerPawn* player = Cast<APlayerPawn>(OtherActor);
	if (player)
	{
		player->LootAmmo(ammoAmount);
		Destroy();
	}
	
}
