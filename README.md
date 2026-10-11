# ScubaCat (Arduino y ESP32)

Animacion del gato buceador en una pantalla OLED SSD1306 I2C de 0,96 pulgadas,
128 x 64 pixeles, para Arduino UNO R4 WiFi, ESP32 DevKit clasico y Arduino UNO R3.
La version para UNO R4 WiFi tambien muestra **ScubAaaaa** desplazandose en su
matriz LED roja integrada mientras la OLED sigue animando al gato.

## Elige tu placa

| Placa | Carpeta que debes abrir | SDA | SCL | Texto en matriz integrada |
|---|---|---|---|---|
| Arduino UNO R4 WiFi | `ScubaCat_R4` | SDA / A4 | SCL / A5 | Si, ScubAaaaa |
| ESP32 DevKit clasico (ESP32-WROOM) | `ScubaCat_ESP32` | GPIO 21 | GPIO 22 | No |
| Arduino UNO R3 (ATmega328P) | `ScubaCat_UNO_R3` | SDA / A4 | SCL / A5 | No |

Cada carpeta incluye su archivo `.ino` y `images.h`. Mantelos juntos.
El ESP32 y el UNO R3 reproducen al gato en la OLED; no tienen la matriz
LED integrada del UNO R4 WiFi.

## Instalacion en Arduino IDE

1. Descarga este repositorio con **Code > Download ZIP** y extraelo.
2. Abre el archivo `.ino` de la carpeta correspondiente a tu placa.
3. Instala desde el Gestor de bibliotecas:
   - **Adafruit SSD1306**.
   - **Adafruit GFX Library**.
   - Acepta las dependencias, incluida **Adafruit BusIO**.
4. Selecciona la placa y el puerto:
   - R4 WiFi: instala **Arduino UNO R4 Boards** y selecciona **Arduino UNO R4 WiFi**.
   - ESP32: instala **esp32 by Espressif Systems** y selecciona **ESP32 Dev Module**.
   - UNO R3: instala **Arduino AVR Boards** y selecciona **Arduino Uno**.
5. Conecta la OLED y pulsa **Subir**.

`Arduino_LED_Matrix` viene incluida en el paquete del UNO R4; no necesitas
ArduinoGraphics ni cables adicionales para sus LEDs.

## Conexion de la OLED

Conecta GND de la OLED a GND y SDA/SCL segun la tabla de arriba.
Respeta los nombres impresos en tu modulo: el orden fisico de los cuatro
pines puede variar entre fabricantes.

- **ESP32:** alimenta la OLED a 3,3 V; las senales I2C tambien son de 3,3 V.
- **UNO R3 y R4 por SDA/SCL:** sus pines son de 5 V. Conecta VCC a 5 V
  solamente si el modulo admite alimentacion y senales I2C a 5 V. Para un
  modulo exclusivamente de 3,3 V, alimentalo a 3,3 V y usa un adaptador de nivel I2C.
- **Alternativa UNO R4 WiFi a 3,3 V:** usa el conector Qwiic y cambia
  `TwoWire &oledWire = Wire;` por `TwoWire &oledWire = Wire1;` en la version R4.
  Qwiic es otro bus y no admite 5 V.

No se puede determinar la compatibilidad electrica por el tamano de 0,96
pulgadas: comprueba las especificaciones de tu modulo.

## Ajustes

- La direccion predeterminada es `0x3C`. Si tu pantalla responde en `0x3D`,
  cambia `SCREEN_ADDRESS` en el `.ino`.
- Esta version requiere SSD1306 de 128 x 64; SH1106 y 128 x 32 requieren ajustes.
- `FRAME_INTERVAL_MS = 60` controla el intervalo del gato.
- En el R4, `LED_TEXT` cambia el texto y `LED_STEP_MS` su velocidad.
- En el ESP32 clasico puedes cambiar los pines en `Wire.begin(21, 22)`.

La reproduccion usa `millis()`, conservando la limpieza de los laterales de
la animacion original. Se reproducen las 15 imagenes disponibles. En UNO R3,
las imagenes y su tabla de punteros se guardan en flash y se leen con las
funciones especificas de AVR para no agotar sus 2 KB de RAM.

## Verificacion

Los resultados de compilacion estan en [VALIDACION.md](VALIDACION.md).
El usuario confirmo que la animacion del gato funciona en su UNO R4 WiFi.
Las versiones nuevas para ESP32 y UNO R3 requieren prueba fisica.

## Creditos

Adaptado del proyecto [Scuba_cat de mlaklaa](https://github.com/mlaklaa/Scuba_cat),
revision `ac292ab0e99083d5284cff411fa0f935020ea870`.
Las imagenes del gato y la animacion base provienen de ese proyecto.
Esta adaptacion agrega soporte para UNO R4 WiFi y UNO R3, el mensaje en la
matriz LED del R4 y el calculo automatico del numero de fotogramas.

Bibliotecas utilizadas: Adafruit SSD1306, Adafruit GFX, Adafruit BusIO,
Wire y Arduino_LED_Matrix (solo en UNO R4 WiFi).

## Referencias de las placas

- [UNO R4 WiFi: pines oficiales](https://docs.arduino.cc/resources/pinouts/ABX00087-full-pinout.pdf).
- [Arduino UNO R3](https://docs.arduino.cc/hardware/uno-rev3/).
- [I2C para Arduino ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/i2c.html).

