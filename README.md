# study-lvgl

LVGL(Light and Versatile Graphics Library) 학습 및 실습 저장소입니다.

## 개요

임베디드 디스플레이 및 스마트워치 UI를 LVGL 로 구현해 보면서
실전 활용 패턴, 위젯 사용법, 이벤트 처리, 애니메이션 등을 익히는 것을 목표로 합니다.

## 디렉토리 구조

| 디렉토리 | 설명 |
| -------- | ---- |
| `01_watch_app/` | Galaxy Watch 사이즈 기준 LVGL 스톱워치 데모 앱 |

## 빌드 환경

- LVGL 9.x
- SDL2 시뮬레이터 (데스크탑 미리보기)
- CMake 3.16 이상
- C11 컴파일러 (clang / gcc)

## macOS 사전 준비

```bash
brew install cmake sdl2
```

## 라이선스

학습용 저장소이며, 별도 명시가 없는 경우 MIT 라이선스를 따릅니다.
