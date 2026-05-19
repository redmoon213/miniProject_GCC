# 보스 공격 판정 구현 전략 (십자 차징 공격)

이 문서는 `EnemyBoss` 클래스의 십자 모양 차징 공격 시 데칼을 활용한 시각적 표현과 실제 물리적 데미지 판정을 연결하는 최적의 전략을 설명합니다.

## 1. 개요
보스의 십자 공격은 시각적으로는 `UDecalComponent`를 통해 표현되지만, 데칼 자체는 충돌 판정 기능이 없습니다. 따라서 공격 시점에 데칼의 크기 정보를 바탕으로 가상의 물리 범위를 체크하는 로직이 필요합니다.

## 2. 권장 방식: `UKismetSystemLibrary::BoxOverlapActors`
별도의 충돌 컴포넌트(BoxComponent 등)를 상시 배치하지 않고, 공격이 발동하는 순간에만 특정 범위를 체크하는 방식입니다.

### 장점
- **리소스 최적화**: 상시 충돌 체크를 하지 않아 성능상 이점이 있습니다.
- **계층 구조 간소화**: 에디터 상에 불필요한 충돌 박스를 배치할 필요가 없습니다.
- **유연성**: 데칼의 크기 변수(`maxDecalLength`, `decalWidth`)를 그대로 활용하여 판정 범위를 일치시키기 쉽습니다.

## 3. 구현 프로세스

### 가. 헤더 파일 준비
`BoxOverlapActors`를 사용하기 위해 필요한 헤더를 소스 파일에 포함합니다.
```cpp
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
```

### 나. 판정 로직 설계 (ExecuteCrossCharge)
1.  **범위 설정**: 데칼의 길이를 기반으로 가로/세로 두 개의 박스 범위(`FVector BoxExtent`)를 계산합니다.
2.  **오버랩 체크**: `UKismetSystemLibrary::BoxOverlapActors` 함수를 두 번 호출(가로, 세로)하여 범위 내 액터들을 수집합니다.
3.  **데미지 적용**: 수집된 액터 중 플레이어를 식별하여 `UGameplayStatics::ApplyDamage`를 호출합니다.

## 4. 예시 구현 코드
```cpp
void AEnemyBoss::ExecuteCrossCharge()
{
    bIsCharging = false;
    HideAllDecals(); // 데칼 숨김 처리

    // 1. 공격 범위 계산 (데칼 크기와 동일하게 설정)
    // 박스 범위는 중심점에서부터의 거리(Extent)이므로 / 2.0f 처리
    FVector boxExtentH = FVector(maxDecalLength / 2.0f, decalWidth / 2.0f, 100.0f);
    FVector boxExtentV = FVector(decalWidth / 2.0f, maxDecalLength / 2.0f, 100.0f);
    
    TArray<AActor*> overlappedActors;
    TArray<AActor*> ignoreActors;
    ignoreActors.Add(this);

    // 2. 가로/세로 범위 내 액터 수집
    UKismetSystemLibrary::BoxOverlapActors(GetWorld(), GetActorLocation(), boxExtentH, 
        TArray<TEnumAsByte<EObjectTypeQuery>>(), nullptr, ignoreActors, overlappedActors);
    
    UKismetSystemLibrary::BoxOverlapActors(GetWorld(), GetActorLocation(), boxExtentV, 
        TArray<TEnumAsByte<EObjectTypeQuery>>(), nullptr, ignoreActors, overlappedActors);

    // 3. 플레이어 데미지 적용
    for (AActor* Actor : overlappedActors)
    {
        if (Actor && Actor == player)
        {
            UGameplayStatics::ApplyDamage(Actor, 1.0f, GetController(), this, nullptr);
            break; // 플레이어는 하나이므로 루프 종료
        }
    }
}
```

## 5. 결론
데칼만으로는 오버랩 체크가 불가능하므로, 위와 같은 **즉시 판정(Instant Check)** 방식을 사용하는 것이 언리얼 엔진의 렌더링-물리 분리 구조에 가장 부합하며 관리 효율성도 높습니다.
