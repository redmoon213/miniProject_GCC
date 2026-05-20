// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "PortalIndicator.h"
#include "Kismet/GameplayStatics.h"

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
	
	indicatorInstance = CreateWidget<UPortalIndicator>(GetWorld(), indicatorClass);
	
	if (indicatorInstance)
	{
		indicatorInstance->SetTarget(this);
		indicatorInstance->AddToViewport();
		
		indicatorInstance->SetRenderOpacity(0.0f);
	}
	
	if (boxComp)
	{
		boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnPortalOverlap);
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortal::ShowPortal()
{
	if (decalComp)
	{
		decalComp->SetHiddenInGame(false);
		indicatorInstance->SetRenderOpacity(1.0f);
		bIsPortalOpen = true;
	}
}

void APortal::OnPortalOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsPortalOpen)
	{
		return;
	}
	
	if (OtherActor&& OtherActor->IsA(APawn::StaticClass()))
	{
		if (!NextLevelName.IsNone())
		{
			UGameplayStatics::OpenLevel(GetWorld(), NextLevelName);
		}
		
	}
}
