#include <Arduino.h>
#include "FastLED.h"
#include "effects.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>

#ifndef STASSID
//#define STASSID "TP-Link_DB4C"
#define STASSID "WifiNet4"
#define STAPSK  "22349026"
#endif
#define EEPROM_SIZE 1


const char* ssid = STASSID;
const char* password = STAPSK;

// Подключаем библиотеку FastLED.

typedef void (*GenericFP)(); //function pointer prototype to a function which takes an 'int' an returns 'void'

void shift();
void effect1();
void effect2();
void effect3();
void effect4();
void effect5();
void keyPressed();
void debounceTime();


GenericFP effects[5] = {&effect1, &effect2, &effect3, &effect4, &effect5}; //create an array of 'GenericFP' function pointers. Notice the '&' operator

// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 180

// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 13
#define BUTTON_PIN 12

// Создаем переменную strip для управления нашей лентой.
CRGB strip[LED_COUNT];

bool buttonState;
int currentEffectIndex;
int debounceTimeCounter;

bool doBreaked;

long startTimer;

void setup()
{
  delay(3000);

    Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    Serial.println("Start");  //  "Начало OTA-апдейта"
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");  //  "Завершение OTA-апдейта" 
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    //  "Ошибка при аутентификации"
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    //  "Ошибка при начале OTA-апдейта"
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    //  "Ошибка при подключении"
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    //  "Ошибка при получении данных"
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    //  "Ошибка при завершении OTA-апдейта"
  });
  ArduinoOTA.begin();
  Serial.println("Ready");  //  "Готово"
  Serial.print("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());
  
  //EEPROM.begin(EEPROM_SIZE);

  // Добавляем ленту
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(strip, LED_COUNT);

  randomSeed(analogRead(0));

  buttonState = false;

  //currentEffectIndex = EEPROM.read(0);

  //currentEffectIndex = 0;
  debounceTimeCounter = 0;

  doBreaked = false;



  attachInterrupt(BUTTON_PIN, keyPressed, HIGH);
}

void ICACHE_RAM_ATTR keyPressed()
{
  timer1_isr_init();
  timer1_attachInterrupt(debounceTime);
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
  timer1_write(10000);
}

void ICACHE_RAM_ATTR debounceTime()
{
  if (digitalRead(BUTTON_PIN) == HIGH)
  {
    Serial.println(millis() - startTimer);
    doBreaked = true;
    timer1_disable();
    currentEffectIndex++;
    //EEPROM.write(0, currentEffectIndex);
    //EEPROM.commit();
    //system_restart();
  }
}

void loop()
{
  ArduinoOTA.handle();

  if (doBreaked)
  {
    //currentEffectIndex++;

    if (currentEffectIndex == sizeof(effects) / sizeof(int))
    {
      currentEffectIndex = 0;
    }

    doBreaked = false;
  }

  effects[currentEffectIndex]();
}

void shift()
{
  for (int i = LED_COUNT; i > 0; i--)
  {
    strip[i] = strip[i - 1];
  }
}

void effect1()
{
  rainbowCycle(strip, 20);
}

void effect2()
{
  Fire(strip, 55, 120, 15);
}

void effect3()
{
  SnowSparkle(strip, 0x10, 0x10, 0x10, 50, random(50, 500));
}

void effect4()
{
  setAll(strip, 0xff, 0xff, 0xff);
  delay(500);
}

void effect5()
{
  setAll(strip, 0, 0, 0);

}

