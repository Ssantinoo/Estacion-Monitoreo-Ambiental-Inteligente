// === Librerías ===
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>  // <--- BT

// === LCD I2C ===
LiquidCrystal_I2C lcd(0x27, 16, 2);

// === DHT11 ===
#define DHT_PIN 8
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

// === Pines analógicos ===
#define ldrPin A1
#define pinMQ5 A0
#define pinKY037 A2

// === RTC DS1302 ===
ThreeWire myWire(3, 2, 4); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

// === SD ===
const int chipSelect = 10;
bool sdOK = false;

// === Bluetooth HC-05 ===
SoftwareSerial BT(6, 5);  // RX = 6, TX = 5

// === Control de tiempo ===
unsigned long lastSave = 0;
const long sampleInterval = 60000;  // 1 minuto

// === Control de Rotación LCD ===
const long lcdRotationInterval = 4000;
unsigned long previousLcdMillis = 0;
int lcdState = 0;
const int numLcdStates = 5;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  Serial.println(F("INICIO"));

  dht.begin();
  lcd.init();
  lcd.backlight();
  Rtc.Begin();
  delay(500);

  if (Rtc.GetIsWriteProtected()) Rtc.SetIsWriteProtected(false);
  
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__); 
  Rtc.SetDateTime(compiled);

  // === Inicialización SD ===
  lcd.clear();
  lcd.print(F("Iniciando SD..."));
  if (SD.begin(chipSelect)) {
    sdOK = true;
    lcd.setCursor(0, 1);
    lcd.print(F("SD: OK"));

    RtcDateTime now = Rtc.GetDateTime();
    if (now.IsValid()) logHeaderToSD(now);
  } else {
    lcd.setCursor(0, 1);
    lcd.print(F("SD: ERROR"));
  }

  mostrarFechaHora();
  previousLcdMillis = millis();
  Serial.println(F("Setup completo"));
}

void loop() {
  unsigned long currentMillis = millis();

  // === Guardado + Envío Bluetooth ===
  if (currentMillis - lastSave >= sampleInterval) {
    guardarDatosYEnviar();
    lastSave = currentMillis;
  }

  // === Rotación del LCD ===
  if (currentMillis - previousLcdMillis >= lcdRotationInterval) {
    previousLcdMillis = currentMillis;
    lcdState = (lcdState + 1) % numLcdStates;
    switch (lcdState) {
      case 0: mostrarFechaHora(); break;
      case 1: mostrarTempHumedad(); break;
      case 2: mostrarLuz(); break;
      case 3: mostrarGas(); break;
      case 4: mostrarSonido(); break;
    }
  }
}

// === FUNCIONES DE VISUALIZACIÓN ===
void mostrarFechaHora() {
  lcd.clear();
  RtcDateTime now = Rtc.GetDateTime();
  if (!now.IsValid()) {
    lcd.print(F("RTC NO VALIDO"));
    return;
  }
  lcd.setCursor(0, 0);
  lcd.print(F("FECHA "));
  lcd.print(now.Day()); lcd.print(F("/")); lcd.print(now.Month()); lcd.print(F("/")); lcd.print(now.Year());
  lcd.setCursor(0, 1);
  lcd.print(F("HORA "));
  if (now.Hour() < 10) lcd.print(F("0")); lcd.print(now.Hour());
  lcd.print(F(":"));
  if (now.Minute() < 10) lcd.print(F("0")); lcd.print(now.Minute());
  lcd.print(F(":"));
  if (now.Second() < 10) lcd.print(F("0")); lcd.print(now.Second());
}

void mostrarTempHumedad() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  lcd.clear();
  if (isnan(h) || isnan(t)) { lcd.print(F("Error DHT11")); return; }
  lcd.setCursor(0, 0);
  lcd.print(F("Temp: ")); lcd.print(t); lcd.print((char)223); lcd.print(F("C"));
  lcd.setCursor(0, 1);
  lcd.print(F("Humedad: ")); lcd.print(h); lcd.print(F("%"));
}

void mostrarLuz() {
  int luz = analogRead(ldrPin);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Luz: ")); lcd.print(luz);
  lcd.setCursor(0, 1);
  if (luz < 200) lcd.print(F("Muy oscuro"));
  else if (luz < 600) lcd.print(F("Luz media"));
  else lcd.print(F("Muy claro"));
}

void mostrarGas() {
  int gas = analogRead(pinMQ5);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Gas: ")); lcd.print(gas);
  lcd.setCursor(0, 1);
  if (gas > 400) lcd.print(F("⚠ Gas detectado!"));
  else lcd.print(F("Nivel normal"));
}

void mostrarSonido() {
  int sonido = analogRead(pinKY037);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Sonido: ")); lcd.print(sonido);
  lcd.setCursor(0, 1);
  if (sonido < 200) lcd.print(F("Tranquilo"));
  else if (sonido < 600) lcd.print(F("Medio"));
  else lcd.print(F("Ruido alto!"));
}

// === FUNCIONES DE UTILIDAD ===
void getFilename(char *filename, RtcDateTime now) {
  sprintf(filename, "%04d%02d%02d.CSV", now.Year(), now.Month(), now.Day());
}

void logHeaderToSD(RtcDateTime now) {
  char filename[13];
  getFilename(filename, now);
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    if (dataFile.size() == 0)
      dataFile.println(F("timestamp,temp_c,hum,light,gas_mq5,sound_ky037"));
    dataFile.close();
  }
}

// === FUNCIÓN PRINCIPAL: Guardar + Enviar por Bluetooth ===
void guardarDatosYEnviar() {
  if (!sdOK) return;

  RtcDateTime now = Rtc.GetDateTime();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int luz = analogRead(ldrPin);
  int gas = analogRead(pinMQ5);
  int sonido = analogRead(pinKY037);

  if (!now.IsValid() || isnan(t) || isnan(h)) return;

  // === Generar timestamp ===
  char timestamp[20];
  sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d",
          now.Year(), now.Month(), now.Day(),
          now.Hour(), now.Minute(), now.Second());

  // === Guardar en SD ===  
  char filename[13];
  getFilename(filename, now);
  File f = SD.open(filename, FILE_WRITE);
  if (f) {
    f.print(timestamp); f.print(F(","));
    f.print(t, 2); f.print(F(","));
    f.print(h, 2); f.print(F(","));
    f.print(luz); f.print(F(","));
    f.print(gas); f.print(F(","));
    f.println(sonido);
    f.close();
  }

  // === Enviar por Bluetooth ===
  BT.print(F("[")); BT.print(timestamp); BT.println(F("]"));
  BT.print(F("Temp: ")); BT.print(t, 1); BT.print(F("°C | "));
  BT.print(F("Hum: ")); BT.print(h, 1); BT.print(F("% | "));
  BT.print(F("Luz: ")); BT.print(luz); BT.print(F(" | "));
  BT.print(F("Gas: ")); BT.print(gas); BT.print(F(" | "));
  BT.print(F("Sonido: ")); BT.println(sonido);
  BT.println(F("-----------------------------------"));

  // === También imprimir en Serial Monitor ===
  Serial.print(F("[")); Serial.print(timestamp); Serial.println(F("]"));
  Serial.print(F("Temp: ")); Serial.print(t, 1); Serial.print(F("°C | "));
  Serial.print(F("Hum: ")); Serial.print(h, 1); Serial.print(F("% | "));
  Serial.print(F("Luz: ")); Serial.print(luz); Serial.print(F(" | "));
  Serial.print(F("Gas: ")); Serial.print(gas); Serial.print(F(" | "));
  Serial.print(F("Sonido: ")); Serial.println(sonido);
  Serial.println(F("-----------------------------------"));

  Serial.println(F("Datos guardados, enviados por BT y mostrados en Serial."));
}
