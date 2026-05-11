// Fill out your copyright notice in the Description page of Project Settings.


#include "miniProject/Public/PlayerPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	SetRootComponent(boxComp);
	
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My StaticMesh Component"));
	meshComp->SetupAttachment(boxComp);

	FVector boxSize = FVector(50, 50, 50);
	boxComp->SetBoxExtent(boxSize);
	
	
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("My Spring Arm Component"));
	springArmComp->SetupAttachment(boxComp);
	
	springArmComp->TargetArmLength = 700.0f;
	springArmComp->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	springArmComp->bUsePawnControlRotation = false;
	springArmComp->bInheritPitch = false;
	springArmComp->bInheritRoll = false;
	springArmComp->bInheritYaw = false;
	
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("My Camera"));
	cameraComp->SetupAttachment(springArmComp, USpringArmComponent::SocketName);
	
	//부드러운 추적을 위한 설정
	springArmComp->bEnableCameraLag = true;
	springArmComp->CameraLagSpeed = 5.0f;  // 값이 느릴수록 부드럽고 천천히 따라옴
	
	
	
	
}	

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	
}

