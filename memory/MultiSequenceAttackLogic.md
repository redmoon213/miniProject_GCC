# 다중 중첩 공격 시스템 구현 가이드 (Multiple Overlapping Attacks)

이 문서는 보스가 여러 세트의 공격(예: 십자 차징)을 순차적으로 발사하되, 각 공격이 서로의 상태를 덮어쓰지 않고 독립적으로 동작하게 만드는 설계 전략을 설명합니다.

## 1. 문제 분석: 왜 기존 방식으로는 중첩이 안 되는가?
현재 `AEnemyBoss` 클래스에 구현된 방식은 다음과 같은 한계가 있습니다.
- **단일 상태 변수**: `bIsCharging`, `currentChargeTime` 등 변수가 클래스에 하나씩만 존재합니다.
- **상태 덮어쓰기**: 새로운 공격이 시작되면(SetTimer 등) 기존 변수값이 초기화되어 이전의 차징 진행도가 사라집니다.
- **컴포넌트 공유**: 4개의 데칼 컴포넌트를 모든 공격 세트가 공유하므로, 시각적 표현이 엉키게 됩니다.

## 2. 해결책: 독립 액터(Actor) 스폰 방식 (가장 권장)
공격 한 세트의 로직(차징, 데칼 표시, 데미지 판정)을 담당하는 별도의 액터 클래스를 만들고, 보스는 이를 스폰하기만 하는 구조입니다.

### 장점
- **완전한 독립성**: 각 액터가 자기만의 변수와 데칼을 가지므로 무한히 중첩 실행이 가능합니다.
- **생명주기 관리**: 공격이 끝나면 액터가 스스로 `Destroy()` 되어 메모리 관리가 자동화됩니다.
- **확장성**: 회전하는 공격, 이동하는 공격 등 복잡한 연출을 액터 단위로 쉽게 구현할 수 있습니다.

## 3. 구현 단계 예시

### 가. 공격 전용 액터 클래스 (`ABossCrossAttack`)
이 클래스는 기존 보스에 있던 데칼 관련 멤버 변수와 `UpdateCharge`, `ExecuteAttack` 로직을 그대로 가져갑니다.

```cpp
// BossCrossAttack.cpp 내부 예시
void ABossCrossAttack::Tick(float DeltaTime)
{
    currentChargeTime += DeltaTime;
    float Percent = currentChargeTime / maxChargeTime;
    
    // 자기 자신의 데칼 머티리얼만 업데이트
    UpdateDecals(Percent);

    if (Percent >= 1.0f) {
        ExecuteDamage(); // 판정 후
        Destroy();       // 스스로 소멸
    }
}
```

### 나. 보스 클래스에서의 순차 호출 로직
보스는 `SetTimer`를 사용하여 공격 액터를 일정 간격과 각도로 스폰만 담당합니다.

```cpp
// EnemyBoss.cpp 내부 예시
void AEnemyBoss::SpawnAttackSequence()
{
    float RotationStep = 20.0f; // 각 세트마다 틀어질 각도
    
    for (int32 i = 0; i < 5; i++)
    {
        FTimerHandle Handle;
        float Delay = i * 0.5f; // 0.5초 간격
        float TargetRot = i * RotationStep;

        // 람다를 사용하여 독립적인 각도 전달
        GetWorldTimerManager().SetTimer(Handle, [this, TargetRot]() {
            FTransform SpawnTM;
            SpawnTM.SetLocation(GetActorLocation());
            SpawnTM.SetRotation(FRotator(0, TargetRot, 0).Quaternion());
            
            // 공격 액터 생성 (각자 독립적으로 동작 시작)
            GetWorld()->SpawnActor<ABossCrossAttack>(AttackClass, SpawnTM);
        }, Delay, false);
    }
}
```

## 4. 대안: 구조체 배열 방식 (객체 생성이 부담될 경우)
보스 내부에 `TArray<FOverlapAttackData>` 배열을 두고, `Tick`에서 이 배열을 순회하며 모든 공격 세트를 수동으로 업데이트하는 방식입니다. 하지만 컴포넌트 동적 생성 및 관리가 까다로워 소규모 프로젝트에서는 **액터 스폰 방식**이 훨씬 효율적입니다.

## 5. 결론
"앞선 공격이 끝나지 않았는데 다음 공격이 시작되어야 하는 상황"에서는 상태 정보를 담는 **인스턴스를 분리(액터 스폰)**하는 것이 언리얼 엔진의 객체지향 구조에 가장 적합한 설계입니다.
