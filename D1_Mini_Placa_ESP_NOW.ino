///////////////////////////////////
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

Adafruit_SSD1306 oled(128, 64, &Wire, -1);

#define led 2  // OJO: En el D1 mini, el led interno, en el GPIO2 se activa en Bajo, es decir, en LOW se enciende y en HIGH de apaga.
#define puls_izda 13
#define puls_dcha 14
bool puls_izda_state = true;
bool puls_dcha_state = true;
uint32_t tmp_puls = 0;
uint32_t tmp_led = 0;


uint8_t MAC_receptor[] = { 0x1A, 0x1A, 0x1A, 0x1A, 0x1A 0x1A };
uint32_t tmp_envio = 0;
bool envio = false;
int status_envio = 0;
int contador = 0;
int mensaje = 0;
uint32_t tmp_borra_msj = 0;

typedef struct {
  int valor;
} Datos;

Datos datos_;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  status_envio = sendStatus;
  envio = true;
}


void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(puls_izda, INPUT_PULLUP);
  pinMode(puls_dcha, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  inicio_led();
  inicio_oled();
  delay(1000);
  Serial.println("\nUP THE IRONS!");
  init_esp_now();
}


void loop() {
  func_puls();
  func_envio();
  func_status_envio();
}


void inicio_led() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(led, !digitalRead(led));
    delay(200);
  }
  digitalWrite(led, HIGH);
}


void inicio_oled() {
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oled.clearDisplay();
  oled.drawRect(0, 0, 128, 16, 1);
  oled.drawRect(0, 17, 128, 47, 1);
  oled.setTextSize(1);
  oled.setTextColor(1);
  oled.setCursor(25, 5);
  oled.print(F("UP THE IRONS!"));
  oled.display();
}


void init_esp_now() {
  if (esp_now_init() != 0) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  } else {
    Serial.println("ESP-NOW Iniciado");
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(MAC_receptor, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}


void func_puls() {
  if (millis() > tmp_puls + 50) {
    tmp_puls = millis();
    puls_dcha_state = !digitalRead(puls_dcha);
    puls_izda_state = !digitalRead(puls_izda);
  }
}


void func_status_envio() {
  if (envio) {
    tmp_borra_msj = millis();
    envio = false;
    Serial.print("Estado envío: ");
    Serial.println(status_envio == 0 ? "OK" : "Error");
    oled.setTextSize(2);
    oled.setTextColor(1, 0);
    oled.setCursor(25, 25);
    oled.print(status_envio == 0 ? "OK   " : "Error");
    oled.display();
  } else {
    func_borra_mensaje();
  }
}


void func_envio() {
  if (puls_dcha_state) {
    puls_dcha_state = !puls_dcha_state;
    mensaje = 876543;
  }
  if (puls_izda_state) {
    puls_izda_state = !puls_izda_state;
    mensaje = 885190;
  }
  if (mensaje != 0) {
    tmp_puls = millis() + 2000;
    digitalWrite(led, LOW);
    tmp_led = millis();
    if (millis() > tmp_envio + 5000) {
      tmp_envio = millis();
      datos_.valor = mensaje;
      esp_now_send(MAC_receptor, (uint8_t *)&datos_, sizeof(datos_));
      Serial.println("Envío: " + String(datos_.valor));
    }
    mensaje = 0;
  }
  if (millis() > tmp_led + 3000) {
    digitalWrite(led, HIGH);
  }
}


void func_borra_mensaje() {
  if (millis() - tmp_borra_msj >= 6000) {
    oled.setTextSize(2);
    oled.setTextColor(1, 0);
    oled.setCursor(25, 25);
    oled.print("     ");
    oled.display();
  }
}
