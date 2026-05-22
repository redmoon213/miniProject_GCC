// Fill out your copyright notice in the Description page of Project Settings.


#include "miniProject/Public/PlayerPawn.h"

#include "BulletPlayerBasic.h"
#include "BulletPlayerSpiral.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyPlayerController.h"
#include "MyGameInstance.h"
#include "PlayerCursor.h"
#include "PlayerHealthUI.h"
#include "PlayerMagUI.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

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

	// 수정 전 코드
	/*
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
		
		// 입력 모드를 게임과 UI 모두 가능하도록 설정 (재시작 시 조작 불능 해결)
		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		pc->SetInputMode(inputMode);
		
		UEnhancedInputLocalPlayerSubsystem* subsys =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys != nullptr)
		{
			subsys->AddMappingContext(imcPlayerInput, 0);
		}
	}
	*/

	// 현재 레벨 이름을 확인합니다.
	FString currentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// 메인 메뉴 레벨이라면 플레이어 관련 UI와 입력을 활성화하지 않습니다.
	if (currentLevelName == "MainMenu")
	{
		return;
	}

	// 위젯 생성코드는 무조건 BeginPlay안에 넣어야함 생성자에 넣으면 크래시 난다고함
	
	playerCursorInstance = CreateWidget<UPlayerCursor>(GetWorld(), playerCursorClass);
	
	//플레이어 체력바 UI
	playerHealthUIInstance =  CreateWidget<UPlayerHealthUI>(GetWorld(), playerHealthUIClass);
	if (playerHealthUIInstance != nullptr)
	{
		playerHealthUIInstance->AddToViewport();
		playerHealthUIInstance->UpdateHealthIcon(playerHp);
	}
	
	// Q 스킬 (탄창) UI 생성
	if (qSkillUiClass)
	{
		qSkillUiInstance = CreateWidget<UPlayerMagUI>(GetWorld(), qSkillUiClass);
		if (qSkillUiInstance)
		{
			qSkillUiInstance->AddToViewport();
			qSkillUiInstance->SetVisibility(ESlateVisibility::Visible);
			qSkillUiInstance->UpdataAmmo(currentAmmo, maxAmmo);
		}
	}

	// E 스킬 (쿨다운) UI 생성
	if (eSkillUiClass)
	{
		eSkillUiInstance = CreateWidget<UPlayerMagUI>(GetWorld(), eSkillUiClass);
		if (eSkillUiInstance)
		{
			eSkillUiInstance->AddToViewport();
			eSkillUiInstance->SetVisibility(ESlateVisibility::Visible);
			eSkillUiInstance->UpdateSkillCooldown(1.0f);
		}
	}
	
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc != nullptr)
	{
		pc->bShowMouseCursor = true;  // 화면에 마우스 커서가 보이도록 함
		pc->SetMouseCursorWidget(EMouseCursor::Default, playerCursorInstance);
		
		// 입력 모드를 게임과 UI 모두 가능하도록 설정 (재시작 시 조작 불능 해결)
		FInputModeGameAndUI inputMode;
		inputMode.SetHideCursorDuringCapture(false);
		pc->SetInputMode(inputMode);
		
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
	
	// 레벨 이동 간 탄약 로드
	if (UMyGameInstance* gi = Cast<UMyGameInstance>(GetGameInstance()))
	{
		currentAmmo = gi->currentAmmo;
		if (qSkillUiInstance != nullptr)
		{
			qSkillUiInstance->UpdataAmmo(currentAmmo, maxAmmo);
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// E 스킬 쿨다운 업데이트
	if (!bCanUseSkillE)
	{
		currentSkillECoolDown += DeltaTime;
		if (currentSkillECoolDown >= skillECoolDown)
		{
			//currentSkillECoolDown = 0.0f;
			bCanUseSkillE = true;
		}

		// UI 업데이트 (E 스킬 전용 인스턴스 사용)
		if (eSkillUiInstance)
		{
			float cooldownPercent = currentSkillECoolDown / skillECoolDown;
			eSkillUiInstance->UpdateSkillCooldown(cooldownPercent);
		}
	}

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
		
		//eic->BindAction(iaChangeWeapon, ETriggerEvent::Started,this, &APlayerPawn::ChangeWeapon);
		
		
		
		eic->BindAction(iaDash, ETriggerEvent::Triggered, this, &APlayerPawn::Dash);
		
		// 스킬 입력 바인딩
		if (iaUseSkill_Q != nullptr)
		{
			eic->BindAction(iaUseSkill_Q, ETriggerEvent::Started, this, &APlayerPawn::UseSkill);
		}

		if (iaUseSkill_E != nullptr)
		{
			eic->BindAction(iaUseSkill_E, ETriggerEvent::Started, this, &APlayerPawn::UseSkill_E);
		}
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

	// 3. 탄환 생성 위치 및 회전 계산
	FVector spawnLocation = firePosition->GetComponentLocation();
	FRotator spawnRotation = firePosition->GetComponentRotation();

	// 4. 현재 누적된 탄퍼짐(currentSpread)을 좌우(Yaw) 방향으로 적용
	float randomYaw = FMath::RandRange(-currentSpread, currentSpread);
	spawnRotation.Yaw += randomYaw;

	int32 factoryIndex = fireMode - 1;
	if (bulletFactories.IsValidIndex(factoryIndex) && bulletFactories[factoryIndex] != nullptr)
	{
		ABulletPlayerBasic* bulletPlayer = GetWorld()->SpawnActor<ABulletPlayerBasic>(
			bulletFactories[factoryIndex],
			spawnLocation,
			spawnRotation);
	}

	// 6. 탄퍼짐 누적: 발사 후 탄퍼짐 값을 증가시킵니다.
	currentSpread = FMath::Min(currentSpread + spreadIncrement, maxSpread);
	
	// 7. 연사 모드(fireMode 2)일 때만 탄약 소모 및 UI 업데이트
	if (fireMode == 2)
	{
		// Q 스킬 충전용 UI 업데이트
		if (qSkillUiInstance != nullptr)
		{
			qSkillUiInstance->UpdataAmmo(currentAmmo, maxAmmo);
		}

		// GameInstance에 저장
		if (UMyGameInstance* gi = Cast<UMyGameInstance>(GetGameInstance()))
		{
			gi->currentAmmo = currentAmmo;
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
	
	boxComp->SetCollisionProfileName(TEXT("Ghost"));
	damageBoxComp->SetCollisionProfileName(TEXT("Ghost"));
	
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
	boxComp->SetCollisionProfileName(TEXT("Player"));
	
	damageBoxComp->SetCollisionProfileName(TEXT("Player"));
}

///플레이어 피해받을때 처리하는 기능
float APlayerPawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsInvincible||playerHp<=0||(DamageCauser&&DamageCauser->ActorHasTag((FName("Debris")))))
	{
		return 0.0f;
	}
	
	if (playerHealthUIInstance != nullptr)
	{
		playerHealthUIInstance->StartPlayerHitAnimation();
	}
	
	playerHp -= FMath::RoundToInt32(DamageAmount);
	
	if (playerHp <= 0)
	{
		//사망처리
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f, Current HP: %d"), DamageAmount, playerHp);
		
		AMyPlayerController* pc = Cast<AMyPlayerController>(GetController());
		if (pc != nullptr)
		{
			pc->ShowRestartUI();
		}
		
		if (meshComp)
		{
			meshComp->SetVisibility(false);
		}
		
		SetActorEnableCollision(false);
		
		if (moveComp)
		{
			moveComp->StopMovementImmediately();
			moveComp->Deactivate();
		}
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
	
	if (qSkillUiInstance != nullptr)
	{
		qSkillUiInstance->UpdataAmmo(currentAmmo, maxAmmo);
	}

	// GameInstance에 저장
	if (UMyGameInstance* gi = Cast<UMyGameInstance>(GetGameInstance()))
	{
		gi->currentAmmo = currentAmmo;
	}
}

void APlayerPawn::UseSkill()
{
	// 탄약이 최대치일 때만 스킬 사용 가능
	if (currentAmmo < maxAmmo)
	{
		return;
	}

	// 탄약 소모 및 UI 업데이트
	currentAmmo = 0;
	if (qSkillUiInstance != nullptr)
	{
		qSkillUiInstance->UpdataAmmo(currentAmmo, maxAmmo);
	}

	// GameInstance에 저장
	if (UMyGameInstance* gi = Cast<UMyGameInstance>(GetGameInstance()))
	{
		gi->currentAmmo = currentAmmo;
	}

	// 스킬 웨이브 초기화 및 실행
	skillWaveCount = 0;
	ExecuteSkillWave();
}

void APlayerPawn::UseSkill_E()
{
	// 쿨다운 체크
	if (!bCanUseSkillE)
	{
		return;
	}

	// 탄약 체크 등 필요 조건 추가 가능
	if (spiralBulletClass)
	{
		FVector spawnLocation = GetActorLocation();
		FRotator spawnRotation = GetActorRotation();

		GetWorld()->SpawnActor<ABulletPlayerSpiral>(spiralBulletClass, spawnLocation, spawnRotation);
		
		UE_LOG(LogTemp, Warning, TEXT("나선형 스킬(E) 발동!"));

		// 쿨다운 시작
		bCanUseSkillE = false;
		currentSkillECoolDown = 0.0f;
	}
}

void APlayerPawn::ExecuteSkillWave()
{
	skillWaveCount++;

	if (skillWaveCount == 1)
	{
		// 1단계: 3발 발사
		SpawnFanBullets(3, 30.0f);
		
		// 다음 발사 전 딜레이 (0.3초)
		GetWorld()->GetTimerManager().SetTimer(skillTimerHandle, this, &APlayerPawn::ExecuteSkillWave, 0.3f, false);
	}
	else if (skillWaveCount == 2)
	{
		// 2단계: 4발 발사
		SpawnFanBullets(4, 45.0f);
		
		// 마지막 5발 발사 전 가장 큰 딜레이 (0.8초)
		GetWorld()->GetTimerManager().SetTimer(skillTimerHandle, this, &APlayerPawn::ExecuteSkillWave, 0.8f, false);
	}
	else if (skillWaveCount == 3)
	{
		// 3단계: 5발 발사
		SpawnFanBullets(5, 60.0f);
	}
}

void APlayerPawn::SpawnFanBullets(int32 bulletCount, float spreadAngle)
{
	if (bulletCount <= 0 || !bulletFactories.IsValidIndex(0) || bulletFactories[0] == nullptr)
	{
		return;
	}

	FVector spawnLocation = firePosition->GetComponentLocation();
	FRotator baseRotation = firePosition->GetComponentRotation();

	// 부채꼴 계산
	float startAngle = -spreadAngle / 2.0f;
	float angleStep = (bulletCount > 1) ? (spreadAngle / (bulletCount - 1)) : 0.0f;

	for (int32 i = 0; i < bulletCount; i++)
	{
		float currentAngle = startAngle + (angleStep * i);
		FRotator spawnRotation = baseRotation;
		spawnRotation.Yaw += currentAngle;

		GetWorld()->SpawnActor<ABulletPlayerBasic>(bulletFactories[0], spawnLocation, spawnRotation);
	}

	// 발사 효과음 (기존 사운드가 있다면 재생)
	if (fireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, fireSound, GetActorLocation());
	}
}
