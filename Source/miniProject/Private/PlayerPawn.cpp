// Fill out your copyright notice in the Description page of Project Settings.


#include "miniProject/Public/PlayerPawn.h"

#include "BulletPlayerBasic.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCursor.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
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
	
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("My Fire Position Component"));
	firePosition->SetupAttachment(boxComp);
	
	boxComp->SetCollisionProfileName(TEXT("Player"));
	
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
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Tick 당, 매 프레임 마다 호출
	// 사용자 입력 키를 이용해서 
	FVector dir =FVector(h,v,0);
	dir.Normalize(); // 방향 벡터 길이가 1이 되도록 정규화(1로 제한)
	
	// y축(좌우) / z축(상하)를 분리하여 이동할 수 있도록 수정
	// 한축이 막혀도, 다른 축은 계속 이동할 수 있도록 처리하기 위해 분리	
	FVector newVector = dir * movementSpeed* DeltaTime;
	
	SetActorLocation(GetActorLocation() + FVector(0, newVector.Y, 0), true);
	SetActorLocation(GetActorLocation() + FVector(newVector.X, 0, 0), true);
	
	
	//마우스 위치에 따라 캐릭터의 방향이 움직이게 함
	//APlayerController* pc = GetWorld()->GetFirstPlayerController(); 이 방식으로도 가져 올 수 있긴 함
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
			//SetActorRotation(playerLookRotation);
			
			
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
		
		eic->BindAction(iaFire, ETriggerEvent::Triggered, this, &APlayerPawn::Fire);
	}
}

void APlayerPawn::OnInputHorizontal(const struct FInputActionValue& value)
{
	h = value.Get<float>();
}

void APlayerPawn::OnInputVertical(const struct FInputActionValue& value)
{
	v = value.Get<float>();
}

void APlayerPawn::Fire(const struct FInputActionValue& value)
{
	
	ABulletPlayerBasic* bulletPlayer = GetWorld()->SpawnActor<ABulletPlayerBasic>(bulletFactory,
		firePosition->GetComponentLocation(), firePosition->GetComponentRotation());
	
}
