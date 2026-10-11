#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino_LED_Matrix.h>
#include <string.h>
#include "images.h"

// OLED SSD1306 I2C de 128 x 64. Usar SDA/A4 y SCL/A5.
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr uint8_t SCREEN_ADDRESS = 0x3C;
constexpr unsigned long FRAME_INTERVAL_MS = 60;
constexpr size_t FRAME_BYTES = SCREEN_WIDTH * SCREEN_HEIGHT / 8;

// Para el conector Qwiic de 3.3 V, cambiar Wire por Wire1 aqui.
TwoWire &oledWire = Wire;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &oledWire, -1);
size_t currentFrame = 0;
unsigned long previousMillis = 0;


ArduinoLEDMatrix matrix;
GFXcanvas1 ledCanvas(12, 8);
uint8_t ledPixels[8][12] = {};
constexpr char LED_TEXT[] = "ScubAaaaa";
constexpr int16_t LED_TEXT_WIDTH = (sizeof(LED_TEXT) - 1) * 6;
constexpr unsigned long LED_STEP_MS = 90;
int16_t textX = 12;
unsigned long previousLedMillis = 0;

void updateLedText(unsigned long now) {
  if (now - previousLedMillis < LED_STEP_MS) return;
  previousLedMillis = now;

  ledCanvas.fillScreen(0);
  ledCanvas.setCursor(textX, 0);
  ledCanvas.print(LED_TEXT);
  for (uint8_t y = 0; y < 8; ++y) {
    for (uint8_t x = 0; x < 12; ++x) {
      ledPixels[y][x] = ledCanvas.getPixel(x, y) ? 1 : 0;
    }
  }
  matrix.renderBitmap(ledPixels, 8, 12);
  --textX;
  if (textX < -LED_TEXT_WIDTH) textX = 12;
}
void setup() {
  Serial.begin(115200);
  matrix.begin();
  ledCanvas.setTextColor(1);
  ledCanvas.setTextSize(1);
  ledCanvas.setTextWrap(false);
  oledWire.begin();
  oledWire.setClock(400000);

  oledWire.beginTransmission(SCREEN_ADDRESS);
  if (oledWire.endTransmission() != 0) {
    Serial.println("OLED no detectada: revisar cables y direccion 0x3C/0x3D.");
    for (;;) { delay(100); }
  }
  // El bus ya esta iniciado: no volver a inicializarlo desde la biblioteca.
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, true, false)) {
    Serial.println("No se pudo reservar el buffer de la OLED.");
    for (;;) { delay(100); }
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  const unsigned long now = millis();
  updateLedText(now);
  if (now - previousMillis < FRAME_INTERVAL_MS) return;
  previousMillis = now;

  uint8_t *buffer = display.getBuffer();
  // En el UNO R4 la memoria flash se puede leer con memcpy normal.
  memcpy(buffer, epd_bitmap_allArray[currentFrame], FRAME_BYTES);

  // Conservar la limpieza de los laterales del proyecto original.
  for (size_t page = 0; page < SCREEN_HEIGHT / 8; ++page) {
    const size_t offset = page * SCREEN_WIDTH;
    memset(buffer + offset, 0, 32);
    memset(buffer + offset + 96, 0, 32);
  }
  display.display();
  currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
}

