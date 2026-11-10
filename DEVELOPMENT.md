# Development notes — Maramau for Freenove/Freemove FNK01044B 2.8"

This document records the porting work behind this repository so the project can be
understood and continued. It complements `README.md`.

## Target hardware

- Board: Freenove / Freemove ESP32-S3 Display **FNK01044B / FNK0104B** (2.8")
- MCU: ESP32-S3 (N16R8), 16 MB flash, 8 MB PSRAM (AP_3v3)
- Display: 2.8" IPS TFT, ILI9341, 240x320, SPI
- Touch: FT6336U capacitive touch, I2C
- Power: LiPo / USB via TP4054 charge IC
- Other: microSD slot, ES8311 audio, WS2812 NeoPixel, GPS-capable UART

## Pin mapping (as configured)

| Function        | Pin(s)                         | Notes |
|-----------------|--------------------------------|-------|
| TFT SPI MOSI    | 11                             | HSPI |
| TFT SPI MISO    | 13                             | |
| TFT SPI SCLK   | 12                             | |
| TFT CS          | 10                             | |
| TFT DC          | 46                             | |
| TFT RST         | -1 (not used)                  | |
| TFT Backlight   | 45                             | active HIGH |
| Touch I2C SDA   | 16                             | FT6336U |
| Touch I2C SCL   | 15                             | |
| Touch INT       | 17                             | |
| Touch RST       | 18                             | |
| Battery ADC     | 9                              | `BAT_ADC`, 2:1 divider |
| SD (SDMMC)      | CLK 38 / CMD 40 / D0 39        | 1-bit SDMMC; `SD_CS`(21) unused |
| GPS TX / RX     | 43 / 44                        | enabled, unvalidated |
| NeoPixel        | 42                             | |

TFT color order is BGR with inversion enabled. SPI uses the HSPI port.

## Build target

- PlatformIO env: `freenove_fnk01044b_s3`
- Board profile: `esp32-s3-devkitc-1`, 16 MB flash (`default_16MB.csv`, `qio_opi`)
- PSRAM enabled in build flags; `-DARDUINO_USB_MODE=1 -DARDUINO_USB_CDC_ON_BOOT=1`
- `extra_script.py` copies `User_Setup_maramau_freenove_s3_28.h` and `User_Setup_Select.h`
  into the TFT_eSPI library used by the build.
- NimBLE pinned to `1.3.8` (legacy API).

## Changes made for this port

- Added `MARAUDER_FREEMOVE_FNK01044B` board target in `configs.h` enabling
  `HAS_TOUCH`, `HAS_CAP_TOUCH`, `HAS_BT`, `HAS_SCREEN`, `HAS_FULL_SCREEN`,
  `HAS_SD`/`USE_SD`, `HAS_GPS`, `HAS_PSRAM`, `HAS_DIRECT_UPLOAD`, plus
  240x320 display/touch/icon/key definitions and pin assignments.
- Enabled `HAS_BATTERY` + `BATTERY_ADC_PIN 9` for this target.
- Backlight handling for this board uses `pinMode(TFT_BL, OUTPUT); digitalWrite(TFT_BL, HIGH)`,
  because the LCD_BL GPIO is GPIO45.
- `ft6336_init()` (FT6336U) confirmed detected (`ID 0x64`).

### Fixes

- **SD on the FNK01044B is on the SDMMC bus, not SPI.** The card is wired to
  `CLK=38, CMD=40, D0=39` (1-bit SDMMC); `SD_CS=21` (shared SPI with TFT/touch)
  is unused. The earlier `SD.begin(21)` blocked boot ("Initializing..." hang).
  For `MARAUDER_FREEMOVE_FNK01044B`, `SDInterface::initSD()` now uses
  `SD_MMC.setPins(38, 40, 39); SD_MMC.begin("/sdcard", true)` (the `SD` symbol is
  `#define`d to `SD_MMC` for this target). **Validated on hardware:** with an SD
  card inserted, boot no longer prints "Failed to mount SD Card (SDMMC)" and
  `ls /` lists the card root (including the auto-created `/SCRIPTS` directory),
  confirming both read and write work.
- **Battery display**: `BatteryInterface` ADC path now sets `i2c_supported = true`
  so the status bar renders the percentage; `MenuFunctions::battery()` was made
  robust for ADC-derived levels (handles `-1` before first read).
- Build compatibility fixes from earlier bring-up: NimBLE 1.3.8 legacy API,
  `LOAD_GFXFF`/font handling, touch `KEY_*`/`ICON_*` scaling for 240x320,
  `connectionPending`/`operationInProgress` declared `extern` and defined in
  `WiFiScan.cpp`, GPS `extern HardwareSerial Serial2`, removal of a
  `ieee80211_raw_frame_sanity_check` call, and `WiFiScan::clearList()` return for
  unknown types.

## Flashing

Preferred tool: `python -m esptool` (esptool v5.3.0 used during development).
Flash the merged image `firmware/fnk01044b_s3_2.8/maramau_fnk01044b_s3_2.8_FULL.bin`
at `0x0`. Do **not** flash the application-only `*.bin` at `0x0`; that caused a
boot loop until the full (bootloader + partitions + app) image was flashed.

If the device boot-loops after a partial flash, flash bootloader + partitions +
app together, or flash the merged full image.

## Known limitations

- GPS enabled in config but not validated without an attached module.
- Battery percentage is a linear estimate from `BAT_ADC` (3.30–4.15 V); can be
  inaccurate under load. TP4054 `CHRG` is not mapped to an ESP32 GPIO, so no
  charging indicator.
- PSRAM: the module is N16R8 (8 MB **Octal/OPI** PSRAM). The initial build used
  `board_build.arduino.memory_type = qio_qspi`, which is for Quad PSRAM (R2
  variants) and silently disables PSRAM -> "PSRAM not available".
  Changed to `qio_opi` (+ `board_build.psram_type = opi`), which is the correct
  setting for 8 MB Octal PSRAM. **Validated on hardware:** with this change the
  boot log no longer prints "PSRAM not available" (`psramInit()` succeeds in
  `esp32_marauder.ino`), so the 8 MB PSRAM is available and the status bar shows
  `P:` usage.
- USB CDC serial (`/dev/ttyACM0`) was intermittent on the test setup; esptool
  flashing was reliable.

## TODO / roadmap

- Ports for the Freenove ESP32-S3 Display **3.5"** and **4.0"** variants.
- Validate GPS with an attached module; confirm UART pin mapping.
- Improve battery reporting (calibration/averaging); add charging indication.
- General refinement and testing of the 2.8" firmware.

## License and attribution

- Upstream: ESP32Marauder by Just Call Me Koko — MIT license, preserved.
- This port follows the same MIT terms; attribution retained in `LICENSE`.
- Third-party libraries retain their own licenses (note ESPAsyncWebServer is
  LGPL-3.0).
- Freenove board image used for identification only (CC BY-NC-SA 3.0).

This project is not affiliated with or endorsed by Just Call Me Koko,
ESP32Marauder, Freenove, or Freemove.
