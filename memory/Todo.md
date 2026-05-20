# 프로젝트 할 일 목록 (Todo)

## ⏳ 진행 중인 작업 (Current)
- 사망 UI(RestartUI) 구조 설계 및 `CodeMap.md` 반영 완료.
- 다음 단계: C++ 코드 구현 및 블루프린트 연결.

## 🚀 다음 세션 작업 (Next Steps)
1. **RestartUI 클래스 구현 (`Source/miniProject/Public/RestartUI.h` & `.cpp`)**
   - `btn_Restart`, `btn_Quit` 버튼 바인딩 (`meta=(BindWidget)`).
   - `OnRestartClicked` (레벨 재오픈), `OnQuitClicked` (게임 종료) 함수 구현.

2. **MyPlayerController 확장 (`Source/miniProject/Public/MyPlayerController.h` & `.cpp`)**
   - `RestartUIClass` 변수 추가.
   - `ShowRestartUI()` 함수 구현 (UI 생성, 뷰포트 추가, 입력 모드 전환).

3. **PlayerPawn 사망 트리거 연결 (`Source/miniProject/Private/PlayerPawn.cpp`)**
   - `TakeDamage()`에서 체력이 0 이하일 때 `AMyPlayerController::ShowRestartUI()` 호출하도록 수정.
   - 사망 시 캐릭터 외형 숨기기 및 충돌 비활성화 처리.

## 📌 메모
- 블루프린트 위젯 제작 시 버튼 이름을 `btn_Restart`, `btn_Quit`으로 일치시켜야 함.
- 사망 시 `Destroy()`를 바로 호출하지 않고, UI를 통해 재시작 여부를 결정할 때까지 카메라를 유지할 것.
