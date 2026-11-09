//                            USER DEFINED SETTINGS
//   TFT_eSPI setup for maramau - Freemove/Freenove FNK0104B/AB 2.8" touch (ESP32-S3 N16R8 + ILI9341 SPI + FT6336U I2C)
//   Aligned to Freenove FNK0104AB_2.8_240x320_ILI9341.h (TFT_eSPI_Setups_v1.3.zip)

// ##################################################################################
// Section 1. Driver
// ##################################################################################

// FNK0104B: test9 worked with ILI9341_2 on TFT_eSPI 2.5.34; maramau uses its pinned TFT_eSPI.
// ILI9341_DRIVER compiles cleanly here; keep all other confirmed-good settings.
#define ILI9341_DRIVER

// 2.8-inch portrait panel native resolution
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// Colour order/inversion (Freenove FNK0104AB)
#define TFT_RGB_ORDER TFT_BGR  // Blue-Green-Red
#define TFT_INVERSION_ON

// ##################################################################################
// Section 2. Pin assignments (Freenove FNK0104AB/FNK0104B 2.8")
// ##################################################################################

#define TFT_MISO  13
#define TFT_MOSI  11
#define TFT_SCLK  12
#define TFT_CS    10    // Chip select
#define TFT_DC    46    // Data/Command
#define TFT_RST   -1    // Reset tied to board reset
#define TFT_BL    45    // Backlight control (Freenove LCD_BL)
#define TFT_BACKLIGHT_ON HIGH

#define TOUCH_CS  -1    // No resistive touch chip; FT6336U is I2C-only

// ##################################################################################
// Section 3. Fonts (Marauder uses GFXFF/FreeFonts)
// ##################################################################################

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// ##################################################################################
// Section 4. SPI / port
// ##################################################################################

#define SPI_FREQUENCY  40000000
#define USE_HSPI_PORT
