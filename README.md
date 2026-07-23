# ESP-NOW Wireless Communication

Proyecto de comunicación inalámbrica de bajo consumo entre microcontroladores **WEMOS D1 mini** y **ESP32 micro:STEAMaker** usando el protocolo **ESP-NOW**.

## 📋 Descripción del Proyecto

Este repositorio contiene sketches de Arduino para establecer comunicación inalámbrica peer-to-peer entre dos dispositivos ESP utilizando el protocolo ESP-NOW. El proyecto implementa un sistema de emisor-receptor donde:

- **WEMOS D1 Mini**: Actúa como controlador que envía comandos mediante dos botones
- **ESP32 micro:STEAMaker**: Recibe los comandos y ejecuta acciones (sirena, LEDs RGB)

## 🔧 Hardware Utilizado

### Dispositivo Emisor (D1 Mini)
- **WEMOS D1 mini** (basado en ESP8266)
- **Display OLED** (128x64) - SSD1306
- **2 Botones** - GPIO 13 (izquierda) y GPIO 14 (derecha)
- **LED** - GPIO 2 (LED interno del D1)
- **Conexión I2C** para el OLED

### Dispositivo Receptor (ESP32)
- **ESP32 micro:STEAMaker**
- **Display OLED** (128x64) - SSD1306
- **25 LEDs RGB** - NeoPixel en GPIO 13
- **Buzzer/Zumbador** - GPIO 33
- **Conexión I2C** para el OLED

## 📁 Estructura del Proyecto

```
ESP-NOW/
├── D1_Mini_Placa_ESP_NOW.ino           # Sketch del emisor (WEMOS D1 mini)
├── ESP32-microSTEAMakers_ESP_NOW_recibe.ino  # Sketch del receptor (ESP32)
└── README.md                           # Este archivo
```

## 🚀 Funcionalidad

### Emisor (WEMOS D1 Mini)
- Detecta presión de botones con debouncing (50ms)
- Envía mensajes de control mediante ESP-NOW cada 5 segundos
- Muestra estado del envío en OLED ("OK" o "Error")
- LED indica transmisión activa
- Botón derecha: envía código `876543`
- Botón izquierda: envía código `885190`

### Receptor (ESP32 micro:STEAMaker)
- Recibe datos ESP-NOW de forma continua
- Activa sirena (buzzer) y LEDs RGB al recibir comando
- Alterna frecuencia (440 Hz y 330 Hz) cada 250ms
- Cambia color LED entre rojo y azul
- Auto-apagado tras 5 segundos de inactividad
- Muestra información en OLED

## 📦 Librerías Requeridas

### Para WEMOS D1 Mini:
```
- Adafruit_SSD1306      # Control del display OLED
- ESP8266WiFi           # WiFi para D1 mini
- espnow.h              # ESP-NOW en ESP8266 (incluida en core)
```

### Para ESP32:
```
- Adafruit_GFX          # Gráficos para OLED
- Adafruit_SSD1306      # Control del display OLED
- WiFi.h                # WiFi para ESP32
- esp_now.h             # ESP-NOW para ESP32 (incluida en core)
- Adafruit_NeoPixel     # Control de LEDs RGB
```

## ⚙️ Instalación

### 1. Configurar Arduino IDE

**Para WEMOS D1 Mini:**
- Instalar gestor de tarjetas: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
- Seleccionar placa: `LOLIN(WEMOS) D1 mini (ESP8266)`

**Para ESP32:**
- Instalar gestor de tarjetas: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
- Seleccionar placa: `ESP32 Dev Module` o variante según tu hardware

### 2. Instalar Librerías

En Arduino IDE → Gestor de Librerías, buscar e instalar:
- `Adafruit SSD1306`
- `Adafruit GFX Library`
- `Adafruit NeoPixel`

### 3. Programar Dispositivos

1. **D1 Mini**: Cargar `D1_Mini_Placa_ESP_NOW.ino`
2. **ESP32**: Cargar `ESP32-microSTEAMakers_ESP_NOW_recibe.ino`
3. Ambos se conectarán automáticamente por ESP-NOW al iniciar

## 🔌 Esquema de Conexión

### WEMOS D1 Mini
```
GPIO 2  → LED (interno, activo en LOW)
GPIO 13 → Botón Izquierda (INPUT_PULLUP)
GPIO 14 → Botón Derecha (INPUT_PULLUP)
SDA (D4) → OLED SDA (0x3C)
SCL (D3) → OLED SCL
```

### ESP32 micro:STEAMaker
```
GPIO 13 → Datos NeoPixel (25 LEDs)
GPIO 33 → Buzzer
SDA     → OLED SDA (0x3C)
SCL     → OLED SCL
```

## 🛠️ Modificaciones Personalización

- **Cambiar dirección MAC receptora**: Editar línea en D1:
  ```cpp
  uint8_t MAC_receptor[] = { 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A };
  ```

- **Cambiar códigos de comando**: Modificar valores en `func_envio()`:
  ```cpp
  mensaje = 876543;  // Botón derecha
  mensaje = 885190;  // Botón izquierda
  ```

- **Duración del auto-apagado**: Cambiar en ESP32:
  ```cpp
  int durac_apagado = 5000;  // milisegundos
  ```

- **Frecuencias del buzzer**: Editar en `func_sirena()`:
  ```cpp
  tone(buzzer, 440);  // Frecuencia 1
  tone(buzzer, 330);  // Frecuencia 2
  ```

## 📊 Comunicación ESP-NOW

- **Protocolo**: Peer-to-peer, bajo consumo (ideal IoT)
- **Rango**: Hasta 250 metros en línea recta
- **Estructura de datos**: struct con un entero (int)
- **Frecuencia de envío**: Cada 5 segundos (emisor)

## ⚡ Características

✅ Comunicación inalámbrica de bajo consumo  
✅ Feedback visual en OLEDs  
✅ Detección de botones con debouncing  
✅ Indicador de estado de transmisión  
✅ Alarma sonora y visual  
✅ Auto-apagado automático  
✅ Sistema escalable a múltiples dispositivos  

## 📝 Notas Importantes

- ⚠️ El LED del D1 mini está **invertido** (LOW = encendido, HIGH = apagado)
- ⚠️ La dirección MAC debe ser válida (formato: 6 bytes hexadecimales)
- ⚠️ ESP-NOW trabaja en **modo WiFi (no requiere conexión a red)**
- ℹ️ El buzzer produce tonos en 440 Hz (La) y 330 Hz (Mi)
- ℹ️ Los OLEDs usan dirección I2C: `0x3C`

## 🚀 Mejoras Futuras

- [ ] Implementar autenticación ESP-NOW
- [ ] Agregar más sensores
- [ ] Sincronización de hora NTP
- [ ] Almacenamiento en SPIFFS
- [ ] Actualización OTA (Over The Air)
- [ ] Interfaz web de control
- [ ] Manejo de múltiples receptores

## 📄 Licencia

Este proyecto es de código abierto. Úsalo libremente en tus proyectos.

## 👨‍💻 Autor

**Marshall100W**

---

**Última actualización:** Julio 2026