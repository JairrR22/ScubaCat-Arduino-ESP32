#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>
#if defined(ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif
#include "images.h"

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr uint8_t SCREEN_ADDRESS = 0x3C; // Cambiar a 0x3D si tu OLED usa esa direccion.
constexpr unsigned long FRAME_INTERVAL_MS = 60;
constexpr size_t FRAME_BYTES = SCREEN_WIDTH * SCREEN_HEIGHT / 8;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
size_t currentFrame = 0;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // ESP32 DevKit clasico: SDA=21, SCL=22.
  Wire.setClock(400000);
  Wire.beginTransmission(SCREEN_ADDRESS);
  if (Wire.endTransmission() != 0) {
    Serial.println(F("OLED no detectada: revisar cables y direccion 0x3C/0x3D."));
    for (;;) { delay(100); }
  }
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS, true, false)) {
    Serial.println(F("No se pudo reservar el buffer de la OLED."));
    for (;;) { delay(100); }
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  const unsigned long now = millis();
  if (now - previousMillis < FRAME_INTERVAL_MS) return;
  previousMillis = now;
  uint8_t *buffer = display.getBuffer();
#if defined(ARDUINO_ARCH_AVR)
  // El UNO R3 requiere leer tanto el puntero como la imagen desde flash.
  const uint8_t *frame = reinterpret_cast<const uint8_t *>(
      pgm_read_ptr(&epd_bitmap_allArray[currentFrame]));
  memcpy_P(buffer, frame, FRAME_BYTES);
#else
  memcpy(buffer, epd_bitmap_allArray[currentFrame], FRAME_BYTES);
#endif
  for (size_t page = 0; page < SCREEN_HEIGHT / 8; ++page) {
    const size_t offset = page * SCREEN_WIDTH;
    memset(buffer + offset, 0, 32);
    memset(buffer + offset + 96, 0, 32);
  }
  display.display();
  currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
}
