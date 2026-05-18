// Fill out your copyright notice in the Description page of Project Settings.


#include "miniProject/Public/PlayerPawn.h"

#include "BulletPlayerBasic.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCursor.h"
#include "PlayerHealthUI.h"
#include "PlayerMagUI.h"
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
	
	damageBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Box Component"));
	damageBoxComp->SetupAttachment(boxComp);
	
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
	springArmComp->CameraLagSpeed = 40.0f;  // 값이 느릴수록 부드럽고 천천히 따라옴
	
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
	
	//플레이어 체력바 UI
	playerHealthUIInstance =  CreateWidget<UPlayerHealthUI>(GetWorld(), playerHealthUIClass);
	if (playerHealthUIInstance != nullptr)
	{
		playerHealthUIInstance->AddToViewport();
		playerHealthUIInstance->UpdateHealthIcon(playerHp);
	}
	
	//플레이어 탄창 UI
	playerMagUIInstance = CreateWidget<UPlayerMagUI>(GetWorld(), playerMagUIClass);
	if (playerMagUIInstance != nullptr)
	{
		playerMagUIInstance->AddToViewport();
		playerMagUIInstance->SetVisibility(ESlateVisibility::Collapsed);
		playerMagUIInstance->UpdataAmmo(currentAmmo, maxAmmo);
	}
	
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
	
	//피해 받을때 깜빡이게 하기위한 다이나믹마테리얼
	
	UMaterialInterface* baseMaterial = meshComp->GetMaterial(0);
	if (baseMaterial != nullptr)
	{
		dynamicMaterial = meshComp->CreateDynamicMaterialInstance(0, baseMaterial);
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 탄퍼짐 회복 로직: 매 프레임마다 현재 탄퍼짐을 최소치로 줄입니다.
	if (currentSpread > minSpread)
	{
		// FMath::Max: 두 값 중 더 큰 값을 반환하여 최소값 아래로 떨어지지 않게 합니다.
		currentSpread = FMath::Max(currentSpread - (spreadRecoveryRate * DeltaTime), minSpread);
	}
	
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
	
	if (dynamicMaterial != nullptr && bIsInvincible)
	{
		float sineValue = FMath::Sin(GetWorld()->GetTimeSeconds() * 30.0f);
		
		float flashValue = (sineValue > 0.0f) ? 1.0f : 0.0f;
		
		dynamicMaterial->SetScalarParameterValue(TEXT("HitFlash"), flashValue);
		
		
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

///플레이어 이동 구현
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
	
	float vValue = value.Get<float>();
	
	if (vValue != 0 && Controller != nullptr)
	{
		const FRotator rotation = cameraComp->GetComponentRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);
		
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(direction, vValue);
	}
}


/// 플레이어 총알 발사 구현
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
	// 1. 기본총(fireMode 1)일 때만 재사격 쿨타임 체크
	if (fireMode == 1)
	{
		float currentTime = GetWorld()->GetTimeSeconds();
		if (currentTime - lastFireTime < fireDelay)
		{
			return;
		}
		lastFireTime = currentTime;
	}

	// 2. 연사 모드(fireMode 2)일 때만 탄약 체크
	if (fireMode == 2 && currentAmmo <= 0)
	{
		return;
	}

	// 3. 탄환 생성 위치 및 회전 계산
	FVector spawnLocation = firePosition->GetComponentLocation();
	FRotator spawnRotation = firePosition->GetComponentRotation();

	// 4. 현재 누적된 탄퍼짐(currentSpread)을 좌우(Yaw) 방향으로 적용
	// FMath::RandRange(Min, Max): 지정된 범위 내의 랜덤한 실수를 반환합니다.
	float randomYaw = FMath::RandRange(-currentSpread, currentSpread);
	spawnRotation.Yaw += randomYaw;

	// 수정 전 코드
	/*
	if (currentAmmo <= 0)
	{
		return;
	}
	// ... (중략) ...
	currentAmmo--;
	playerMagUIInstance->UpdataAmmo(currentAmmo, maxAmmo);
	*/

	// 5. 변형된 회전값으로 탄환 생성 (TArray 기반으로 복구)
	// ABulletPlayerBasic* bulletPlayer = GetWorld()->SpawnActor<ABulletPlayerBasic>(bulletFactory,
	// 	spawnLocation, spawnRotation);

	int32 factoryIndex = fireMode - 1;
	if (bulletFactories.IsValidIndex(factoryIndex) && bulletFactories[factoryIndex] != nullptr)
	{
		ABulletPlayerBasic* bulletPlayer = GetWorld()->SpawnActor<ABulletPlayerBasic>(
			bulletFactories[factoryIndex],
			spawnLocation,
			spawnRotation);
	}

	// 6. 탄퍼짐 누적: 발사 후 탄퍼짐 값을 증가시킵니다.
	// FMath::Min: 최대치를 넘지 않도록 보정합니다.
	currentSpread = FMath::Min(currentSpread + spreadIncrement, maxSpread);
	
	// 7. 연사 모드(fireMode 2)일 때만 탄약 소모 및 UI 업데이트
	if (fireMode == 2)
	{
		currentAmmo--;
		if (playerMagUIInstance != nullptr)
		{
			playerMagUIInstance->UpdataAmmo(currentAmmo, maxAmmo);
		}
	}
}



/// 플레이어 대시 구현
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

///플레이어 무기 교체 구현
void APlayerPawn::ChangeWeapon(const struct FInputActionValue& value)
{
	float getValue = value.Get<float>();
	
	fireMode = FMath::RoundToInt32(getValue);
	
	if (fireMode == 2)
	{
		playerMagUIInstance->SetVisibility(ESlateVisibility::Visible);
	}
	
	else
	{
		playerMagUIInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

///플레이어 피해받을때 처리하는 기능
float APlayerPawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	
	/*if (DamageCauser&&DamageCauser->ActorHasTag((FName("Debris"))))
	{
		return 0.0f;
	}*/
	if (bIsInvincible||playerHp<=0||(DamageCauser&&DamageCauser->ActorHasTag((FName("Debris")))))
	{
		return 0.0f;
	}
	
	playerHp -= FMath::RoundToInt32(DamageAmount);
	
	if (playerHp <= 0)
	{
		//사망처리
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f, Current HP: %d"), DamageAmount, playerHp);
		Destroy();
	}
	
	else
	{
		if (playerHealthUIInstance != nullptr)
		{
			playerHealthUIInstance->UpdateHealthIcon(playerHp);
		}
		bIsInvincible = true;
		GetWorld()->GetTimerManager().SetTimer(invincibleTimerHandle, this, &APlayerPawn::ResetInvincibility, invincibleDuration, false);

	}
	
	return DamageAmount;
}

void APlayerPawn::ResetInvincibility()
{
	bIsInvincible = false;
	dynamicMaterial->SetScalarParameterValue(TEXT("HitFlash"), 0.0f);
}

void APlayerPawn::LootAmmo(int32 ammoAmount)
{
	currentAmmo = FMath::Min(currentAmmo + ammoAmount, maxAmmo);
	
	playerMagUIInstance->UpdataAmmo(currentAmmo,maxAmmo);
}
