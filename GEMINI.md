# Project Guidelines (miniSampleMudGame)

이 파일은 Gemini CLI가 프로젝트를 수행할 때 준수해야 하는 핵심 지침을 담고 있습니다.

## 1. 명명 규칙 (Naming Conventions)
- **변수명 (Variables):** `camelCase`를 사용합니다. (예: `monsterNumber`, `isAlive`)
- **클래스 및 함수명 (Classes & Functions):** `PascalCase`를 사용합니다. (예: `GameManager`, `EnterNextRoom()`)

## 2. 운영 원칙 (Operational Constraints)
- **코드 수정 금지:** 사용자가 명시적으로 코드 수정을 요청(Directive)하지 않는 한, 절대로 소스 코드를 직접 수정하거나 파일을 덮어쓰지 않습니다. 
- **분석 우선:** 모든 요청에 대해 우선 분석 및 전략 제안(Inquiry)을 수행하고, 사용자의 승인이 있을 때만 실행 단계로 넘어갑니다.
- **기존 코드 보존:** 사용자의 요청으로 코드를 수정할 경우, 기존 코드를 삭제하는 대신 주석 처리합니다. 주석 처리된 코드 상단에는 `// 수정 전 코드`라고 명시합니다.

## 3. 주석 규칙 (Commenting Rules)
- **언어:** 모든 주석(프롬프트 응답 및 에디터 내 코드)은 **한글**로 작성합니다.
- **라이브러리 및 매크로 설명:** 외부/표준 라이브러리 함수나 매크로를 사용할 경우, 해당 기능의 **역할**과 **매개변수의 의미**를 설명하는 상세 주석을 반드시 포함합니다.
