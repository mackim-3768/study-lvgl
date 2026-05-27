# 01_watch_app — Galaxy Watch Stopwatch (LVGL)

LVGL 9.x로 작성한 Galaxy Watch 사이즈(**466 × 466**) 원형 디스플레이 기준 스톱워치 데모.
데스크탑에서는 SDL2 시뮬레이터로 동작한다.

## 기능

- `MM:SS.cc` 포맷 시간 표시 (10ms 해상도)
- Start / Pause / Resume 토글 버튼
- 실행 중에는 Lap, 정지 상태에서는 Reset 로 동작하는 보조 버튼
- 마지막 랩 시간 + 랩 목록 (최근 항목 노출)
- 1분에 한 바퀴 도는 베젤 인디케이터 아크

## 화면 사양

| 항목 | 값 |
| --- | --- |
| 해상도 | 466 × 466 |
| 형태 | 원형(시뮬레이션은 사각 윈도우 안에 원형 다이얼 컴포지팅) |
| 색심도 | 32 bit |

> 참고: Galaxy Watch 4 / 5 / 6 (44 mm) 의 디스플레이 해상도가 466 × 466 이다.

## 빌드 & 실행 (macOS)

```bash
brew install cmake sdl2

cd 01_watch_app
cmake -S . -B build
cmake --build build -j
./build/watch_app
```

처음 빌드 시 LVGL 소스가 FetchContent 로 내려받아진다(`build/_deps/lvgl-src`).
빌드 산출물은 `build/` 안에만 떨어지므로 그 폴더는 `.gitignore` 대상이다.

## 빌드 & 실행 (Linux)

```bash
sudo apt install cmake libsdl2-dev

cd 01_watch_app
cmake -S . -B build
cmake --build build -j
./build/watch_app
```

## 디렉토리

```
01_watch_app/
├── CMakeLists.txt   # LVGL FetchContent + SDL2 링크
├── lv_conf.h        # LVGL 빌드 설정 (466x466, 32bpp, SDL 드라이버)
├── main.c           # SDL 디스플레이/입력 초기화, 메인 루프
└── src/
    ├── stopwatch_ui.h
    └── stopwatch_ui.c   # 스톱워치 UI 와 상태 머신
```

## 조작

| 입력 | 동작 |
| --- | --- |
| 우측(녹/적) 버튼 클릭 | Start ↔ Pause / Resume |
| 좌측 회색 버튼 클릭 | 실행 중이면 Lap, 정지/대기 상태면 Reset |

마우스로 클릭하면 된다 (실 기기에서는 터치 입력에 대응).
