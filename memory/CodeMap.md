# miniProject Code Map

이 문서는 프로젝트의 핵심 소스 코드 구조와 로직을 요약하여, 개발 효율성을 높이기 위해 작성되었습니다.

## 1. 클래스 상속 구조 (Inheritance Hierarchy)

- **AGameModeBase**
  - `AMyGameModeBase`: 게임 상태 관리 (적 스폰, 보스 등장 조건 제어)
- **APlayerController**
  - `AMyPlayerController`: 플레이어 입력 및 특수 UI(보스 경고) 제어
- **APawn**
  - `APlayerPawn`: 플레이어 캐릭터 (이동, 공격, 대시, 체력 관리)
- **AActor**
  - `AEnemy`: 기본 적 AI (플레이어 추적, 차징 공격, 투사체 공격)
    - `AEnemyBoss`: 보스 적 AI (멀티 패턴: 십자 공격, 점프 공격, 회전 투사체)
  - `AEnemyFactory`: 적 및 보스 스폰 로직 관리
  - `ABulletPlayerBasic`: 플레이어 총알
  - `ABulletEnemyBasic`: 적/보스 총알
  - `ADroppedAmmo`: 탄약 보급 아이템
  - `AEnemySpawnIndicator`: 적 스폰 예고 데칼 및 로직
  - `APortal`: 레벨 이동 포탈
  - `AKillZone`: 낙사/데미지 구역
- **UUserWidget**
  - `UPlayerHealthUI`: 플레이어 체력 표시 (하트 아이콘 관리)
  - `UPlayerMagUI`: 탄약 UI (링 형태의 다이내믹 머티리얼 사용)
  - `UEnemyIndicatorWidget`: 화면 밖 적 방향 표시 화살표

---

## 2. 핵심 클래스 상세 설명 (Class Map)

### 🎮 프레임워크 및 플레이어
| 클래스명 | 주요 역할 | 핵심 함수/변수 |
| :--- | :--- | :--- |
| **AMyGameModeBase** | 적 사망 체크, 보스 스폰 트리거 | `OnEnemyDie()`, `SpawnBoss()`, `enemyCount` |
| **APlayerPawn** | 이동, 무기 교체, 대시, 피격 로직 | `Fire()`, `Dash()`, `TakeDamage()`, `currentAmmo`, `playerHp` |
| **AMyPlayerController** | 보스 등장 경고 UI 제어 | `ShowBossSpawnWarning()`, `HideBossSpawnWarning()` |

### 👾 적 (AI) 시스템
| 클래스명 | 주요 역할 | 핵심 함수/변수 |
| :--- | :--- | :--- |
| **AEnemy** | 주사위 눈금 기반 AI, 차징/투사체 공격 | `MoveToPlayer()`, `ChargingExcute()`, `UpdateDiceEye()` |
| **AEnemyBoss** | 복합 패턴 공격 (FSM 형태) | `ChoosePattern()`, `StartCrossCharge()`, `StartJumpAttack()` |
| **AEnemyFactory** | 유효 스폰 위치 계산 및 생성 | `GetValidSpawnLocation()`, `SpawnSingleEnemy()` |

### UI 및 기타
| 클래스명 | 주요 역할 | 핵심 함수/변수 |
| :--- | :--- | :--- |
| **UPlayerHealthUI** | 체력 아이콘 및 피격 비네트 효과 | `UpdateHealthIcon()`, `StartPlayerHitAnimation()` |
| **UPlayerMagUI** | 잔탄량 시각화 (MID 활용) | `UpdataAmmo()` |
| **APortal** | 특정 조건 충족 시 다음 레벨 이동 | `ShowPortal()`, `NextLevelName` |

---

## 3. 핵심 시스템 로직

### ⚔️ 공격 및 데미지 시스템
- **플레이어 공격:** `APlayerPawn`에서 `IA_Fire` 입력 시 `Fire()` 호출. `bulletFactories` 배열에서 현재 무기에 맞는 총알 스폰. 탄퍼짐(`currentSpread`) 로직 포함.
- **적 피격:** `AEnemy::TakeDamage` 호출 시 주사위 눈금(`currentDiceEye`) 감소, 0이 되면 `Die()` 호출 및 탄약(`ADroppedAmmo`) 드랍.
- **플레이어 피격:** `APlayerPawn::TakeDamage` 호출 시 무적 시간(`bIsInvincible`) 적용 및 UI 업데이트.
- **사망 및 재시작:** 
    1. `APlayerPawn` 체력이 0이 되면 `AMyPlayerController::ShowRestartUI()` 호출.
    2. `RestartUI` 생성 및 화면 표시, 입력 모드를 `UIOnly`로 변경.
    3. '재시작' 클릭 시 현재 레벨 재오픈, '종료' 클릭 시 게임 종료.

### 🏃 이동 및 대시
- **플레이어:** `UFloatingPawnMovement` 기반. `Dash()` 시 `UTimelineComponent`와 `UCurveFloat`을 사용하여 부드러운 가속 구현.
- **보스:** `MoveForward()`에서 `hopHeight`를 이용한 점프 이동 표현.

### 🎯 적 스폰 시퀀스
1. `AMyGameModeBase`가 `AEnemyFactory`에 스폰 요청.
2. `AEnemyFactory`가 `GetValidSpawnLocation()`으로 위치 선정 후 `AEnemySpawnIndicator` 생성.
3. `AEnemySpawnIndicator`가 일정 시간 차징 후 실제 `AEnemy` 스폰.

---

## 4. 기술적 규칙 (Summary)
- **명명 규칙:** 변수 `camelCase`, 클래스/함수 `PascalCase` (GEMINI.md 준수).
- **머티리얼:** `UMaterialInstanceDynamic`(MID)을 적극적으로 활용하여 체력, 탄약, 적 상태(피격 플래시)를 시각화.
- **주석:** 모든 코드 주석은 **한글**로 작성.
