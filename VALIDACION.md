# Validacion

Fecha: 11 de octubre de 2026.

## Compilacion

| Placa | Paquete | Resultado | Programa | RAM estatica |
|---|---|---|---|---|
| UNO R4 WiFi | arduino:renesas_uno 1.6.0 | Correcto | 88 608 B (33%) | 9 608 B (29%) |
| UNO R3 | arduino:avr 1.8.8 | Correcto | 29 708 B (92%) | 528 B (25%) |
| ESP32 Dev Module | esp32:esp32 3.3.12 | Correcto | 321 404 B (24%) | 23 676 B (7%) |

Bibliotecas: Adafruit SSD1306 2.5.17, Adafruit GFX 1.12.6 y Adafruit BusIO 1.17.4.
UNO R4 WiFi usa ademas LED_Matrix 1.1.0, incluida en el paquete de placa.

La RAM estatica informada por el compilador no incluye el buffer dinamico de
1 024 bytes que crea Adafruit SSD1306. En UNO R3 esto deja menos de 496 bytes
para otras asignaciones y la pila. Esta version reproduce la animacion sin
agregar funciones ni texto en una matriz externa; comprobar estabilidad en
hardware antes de ampliar el sketch.

## Imagenes

Se comprobo que cada version contiene 15 fotogramas de 1 024 bytes.
Las versiones ESP32 y UNO R3 conservan las imagenes originales. Para UNO R3
la tabla de punteros tambien queda en PROGMEM y se lee mediante pgm_read_ptr.

## Pruebas fisicas

El usuario confirmo que la animacion del gato se ve en su UNO R4 WiFi.
La comprobacion de las versiones nuevas es de compilacion: ESP32, UNO R3 y
el desplazamiento de ScubAaaaa en la matriz LED requieren prueba fisica.

