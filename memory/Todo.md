# 프로젝트 할 일 목록 (Todo)

## ⏳ 진행 중인 작업 (Current)
- **보스 특수 패턴 준비 (Mesh Splitting):**
  - 모델링 모드에서 `Slice` 또는 `Plane Cut` 도구를 사용하여 보스 메쉬 분리 필요.
  - 분리 후 `UpperBody`, `LowerBody` 컴포넌트 구조로 보스 액터 재구성.

- **보스 체력 바 UI 고도화:**
  - `Fill Image` 전용 머티리얼 수정 (오른쪽 잘림 현상 해결을 위한 UV Clamp 적용).
  - 텍스처 배경 투명도 문제 해결 (Alpha 채널이 포함된 PNG 재임포트 또는 마스크 머티리얼 제작).

## 🚀 다음 세션 작업 (Next Steps)
1. **보스 분리 공격(Split-Open Attack) C++ 구현:**
   - `EBossPattern::CoreAttack` 패턴 추가.
   - `UTimelineComponent`를 이용한 몸체 상하 분리 애니메이션 구현.
2. **사망 UI(RestartUI) 블루프린트 연결 및 테스트:**
   - C++에서 구현된 `btn_Restart`, `btn_Quit` 버튼 바인딩 확인 및 기능 테스트.

## 📌 메모
- **운영 원칙 준수:** `GEMINI.md`에 추가된 '기존 코드 존중(Surgical Update)' 원칙에 따라 코드 수정 시 기존 로직 보존 철저.
- **UI 팁:** 프로그레스 바의 `Fill Color and Opacity`를 흰색(1,1,1,1)으로 설정하여 틴트 오류 해결할 것.
