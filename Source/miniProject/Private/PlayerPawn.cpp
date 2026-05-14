// Fill out your copyright notice in the Description page of Project Settings.


#include "miniProject/Public/PlayerPawn.h"

#include "BulletPlayerBasic.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCursor.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
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
	
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("My Fire Position Component"));
	firePosition->SetupAttachment(boxComp);
	
	//충돌채널 프로필 설정
	boxComp->SetCollisionProfileName(TEXT("Player"));
	
	moveComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Movement Component"));
	moveComp->MaxSpeed = movementSpeed;
	
	//대시 기능에 관련된 변수 초기화
	dashTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("My DashTimeline Component"));
	
	
}	

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	// 위젯 생성코드는 무조건 BeginPlay안에 넣어야함 생성자에 넣으면 크래시 난다고함
	playerCursorInstance = CreateWidget<UPlayerCursor>(GetWorld(), playerCursorClass);
	
	
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc != nullptr)
	{
		pc->bShowMouseCursor = true;  // 화면에 마우스 커서가 보이도록 함
		pc->SetMouseCursorWidget(EMouseCursor::Default, playerCursorInstance);
		
		
		UEnhancedInputLocalPlayerSubsystem* subsys =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys != nullptr)
		{
			subsys->AddMappingContext(imcPlayerInput, 0);
		}
	}
	
	//대시 기능에 관련된 기능
	if (dashCurve!=nullptr)
	{
		FOnTimelineFloat progressUpdate;
		progressUpdate.BindUFunction(this, FName("DashProgress"));
		dashTimelineComp->AddInterpFloat(dashCurve, progressUpdate);
		
		FOnTimelineEvent finishedEvent;
		finishedEvent.BindUFunction(this, FName("DashFinished"));
		dashTimelineComp->SetTimelineFinishedFunc(finishedEvent);
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APlayerController* pc = Cast<APlayerController>(GetController());
	
	if (pc != nullptr)
	{
		FHitResult hitResult;
		
		if (pc->GetHitResultUnderCursor(ECC_Visibility, false, hitResult))
		{
			FVector targetLocation = hitResult.ImpactPoint;
			FVector playerLocation = GetActorLocation();
			
			targetLocation.Z = playerLocation.Z;
			
			FRotator playerLookRotation = FRotationMatrix::MakeFromX(targetLocation - playerLocation).Rotator();
			
			
			FRotator currentRotation = GetActorRotation();
			FRotator smoothRotation = FMath::RInterpTo(currentRotation, playerLookRotation, DeltaTime, 10.0f); // 10.0f는 회전 속도
			SetActorRotation(smoothRotation);
		}
		
	}
	
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* eic = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (eic != nullptr)
	{
		eic->BindAction(iaHorizontal, ETriggerEvent::Triggered, this, &APlayerPawn::OnInputHorizontal);
		eic->BindAction(iaHorizontal, ETriggerEvent::Completed, this, &APlayerPawn::OnInputHorizontal);
		
		eic->BindAction(iaVertical, ETriggerEvent::Triggered, this, &APlayerPawn::OnInputVertical);
		eic->BindAction(iaVertical, ETriggerEvent::Completed, this, &APlayerPawn::OnInputVertical);
		
		
		eic->BindAction(iaFire, ETriggerEvent::Started, this, &APlayerPawn::StartFire);
		eic->BindAction(iaFire, ETriggerEvent::Completed, this, &APlayerPawn::EndFire);
		
		eic->BindAction(iaChangeWeapon, ETriggerEvent::Started,this, &APlayerPawn::ChangeWeapon);
		
		eic->BindAction(iaDash, ETriggerEvent::Triggered, this, &APlayerPawn::Dash);
	}
}

void APlayerPawn::OnInputHorizontal(const struct FInputActionValue& value)
{
	//h = value.Get<float>();
	float hValue = value.Get<float>();
	
	if (hValue != 0 && Controller != nullptr)
	{
		const FRotator rotation = cameraComp->GetComponentRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
			
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		
		AddMovementInput(direction, hValue);
	}
}

void APlayerPawn::OnInputVertical(const struct FInputActionValue& value)
{
	//v = value.Get<float>();
	
	float vValue = value.Get<float>();
	
	if (vValue != 0 && Controller != nullptr)
	{
		const FRotator rotation = cameraComp->GetComponentRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(direction, vValue);
	}
}

void APlayerPawn::StartFire()
{
	Fire();
	
	if (fireMode == 2)
	{
		GetWorld()->GetTimerManager().SetTimer(autoFireTimerHandle, this, &APlayerPawn::Fire, autoFireRate, true);
	}
	
	
}

void APlayerPawn::EndFire()
{
	GetWorld()->GetTimerManager().ClearTimer(autoFireTimerHandle);
}


void APlayerPawn::Fire()
{
	
	ABulletPlayerBasic* bulletPlayer = GetWorld()->SpawnActor<ABulletPlayerBasic>(bulletFactory,
		firePosition->GetComponentLocation(), firePosition->GetComponentRotation());
	
}

void APlayerPawn::Dash(const struct FInputActionValue& value)
{
	
	if (isDashing || dashCurve == nullptr)
	{
		return;
	}
	
	isDashing = true;
	
	FVector inputDirection = GetLastMovementInputVector().GetSafeNormal();
	
	FVector currentVelocity = GetVelocity();
	
	if (!inputDirection.IsNearlyZero())
	{
		dashDirection = inputDirection;
	}
	
	else
	{
		dashDirection = GetActorForwardVector();
	}
	
	dashDirection.Z = 0.0f;
	
	dashTimelineComp->PlayFromStart();
	
}

void APlayerPawn::DashProgress(float Value)
{
	float dashBaseSpeed = 1000.0f;
	float deltaTime = GetWorld()->GetDeltaSeconds();
	
	FVector deltaLocation = dashDirection * dashBaseSpeed * Value  * deltaTime;
	
	AddActorWorldOffset(deltaLocation, true);
	
}


void APlayerPawn::DashFinished()
{
	
	isDashing = false;
}

void APlayerPawn::ChangeWeapon(const struct FInputActionValue& value)
{
	float getValue = value.Get<float>();
	
	fireMode = FMath::RoundToInt32(getValue);
	
}
