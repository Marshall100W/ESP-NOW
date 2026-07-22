///////////////////////////////////
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_NeoPixel.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

#define rgb_pin 13
#define NUMPIXELS 25
uint8_t pixel_num = 0;
#define buzzer 33

Adafruit_NeoPixel pixels(NUMPIXELS, rgb_pin, NEO_GRB + NEO_KHZ800);


bool status_recibido = false;
bool envio = false;
esp_now_send_status_t status_envio;
int contador = -1;
int valor_inicial = 876543;

typedef struct {
  int valor;
} Datos;

Datos incomingData;
int dato_recibido = 0;
int durac_apagado = 5000;
uint32_t tmp_apagado = -durac_apagado;
uint32_t tmp_tono = 0;
bool LA = false;

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(Datos));
  status_recibido = true;
  dato_recibido = incomingData.valor;
  contador++;
}


void setup() {
  Serial.begin(115200);
  init_display();
  pixels.begin();
  pixels.clear();
  pinMode(buzzer, OUTPUT);
  WiFi.mode(WIFI_STA);
  delay(500);
  Serial.println("\nUP THE IRONS!");
  inicio_ESP_NOW();
  delay(1000);
}

void loop() {
  func_recibido();
}


void init_display() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawRect(0, 0, 128, 15, SSD1306_WHITE);
  display.drawRect(0, 16, 128, 48, SSD1306_WHITE);
  display.drawRect(32, 29, 64, 28, SSD1306_WHITE);
  display.setCursor(5, 5);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.write("UP THE IRONS!");
  display.display();
}

void inicio_ESP_NOW() {
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  } else {
    Serial.println("ESP-NOW Iniciado");
  }
  esp_now_register_recv_cb(OnDataRecv);
}


void func_recibido() {
  if (status_recibido) {
    status_recibido = false;
    Serial.println("Recibido: " + String(dato_recibido));
    Serial.println("Contador: " + String(contador));
  }
  //if (dato_recibido == 876543 - contador) {
  if (dato_recibido > 875000) {
    delay(50);
    LA = true;
    func_sirena();
    tmp_apagado = millis();
    dato_recibido = 0;

  } else {
    if (millis() > tmp_apagado + durac_apagado) {
      for (int i = 0; i < 25; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
      pixels.show();
      noTone(buzzer);
    } else {
      func_sirena();
    }
  }
}

void func_sirena() {
  if (millis() > tmp_tono + 250) {
    tmp_tono = millis();
    if (LA) {
      tone(buzzer, 440);
      for (int i = 0; i < 25; i++) {
        pixels.setPixelColor(i, pixels.Color(10, 0, 0));
      }
      pixels.show();
    } else {
      tone(buzzer, 330);
      for (int i = 0; i < 25; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 10));
      }
      pixels.show();
    }
    LA = !LA;
  }
}