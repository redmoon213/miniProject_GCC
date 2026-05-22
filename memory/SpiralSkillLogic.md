# Spiral Bullet Skill (E-Key) Implementation Plan

이 문서는 플레이어를 중심으로 회전하며 나선형으로 뻗어나가는 E 스킬의 구현 계획을 담고 있습니다.

## 1. PlayerPawn 수정 사항

### PlayerPawn.h 선언부
```cpp
// 입력 액션 및 총알 클래스 변수
UPROPERTY(EditAnywhere, Category = "Input")
class UInputAction* iaUseSkill_E;

UPROPERTY(EditAnywhere, Category = "Weapon")
TSubclassOf<class AActor> spiralBulletClass;

// 스킬 발동 함수
void UseSkill_E();
```

### PlayerPawn.cpp 구현부
```cpp
// SetupPlayerInputComponent 내부 바인딩
if (iaUseSkill_E)
{
    eic->BindAction(iaUseSkill_E, ETriggerEvent::Started, this, &APlayerPawn::UseSkill_E);
}

// 함수 로직
void APlayerPawn::UseSkill_E()
{
    // 탄약 체크 등의 조건 (필요 시 추가)
    if (currentAmmo < 5) return; // 예시: 탄약 5발 필요
    
    if (spiralBulletClass)
    {
        FVector spawnLocation = GetActorLocation();
        FRotator spawnRotation = GetActorRotation();

        AActor* spiralBullet = GetWorld()->SpawnActor<AActor>(spiralBulletClass, spawnLocation, spawnRotation);
        
        if (spiralBullet)
        {
            // 스킬 사용 피드백 (로그, 사운드 등)
            UE_LOG(LogTemp, Warning, TEXT("Spiral Skill Activated!"));
        }
    }
}
```

## 2. 이동 로직 핵심 공식
나선형 이동은 매 프레임마다 **각도(Angle)**와 **반지름(Radius)**을 동시에 증가시켜 구현합니다.

- `X = Center.X + Cos(Angle) * Radius`
- `Y = Center.Y + Sin(Angle) * Radius`
- `Angle += RotationSpeed * DeltaTime`
- `Radius += ExpansionSpeed * DeltaTime`
