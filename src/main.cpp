#include <Arduino.h>
#include "FastLED.h"
// Подключаем библиотеку FastLED.

typedef void (* GenericFP)(); //function pointer prototype to a function which takes an 'int' an returns 'void'

void shift();
void effect1();
void effect2();
void effect3();
bool isPressed();

GenericFP effects[3] = {&effect1, &effect2, &effect3}; //create an array of 'GenericFP' function pointers. Notice the '&' operator

// Указываем, какое количество пикселей у нашей ленты.
#define LED_COUNT 180
 
// Указываем, к какому порту подключен вход ленты DIN.
#define LED_PIN 13
#define BUTTON_PIN 12
 
// Создаем переменную strip для управления нашей лентой.
CRGB strip[LED_COUNT];

bool buttonState;
int currentEffectIndex;

 
void setup()
{
  // Добавляем ленту
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(strip, LED_COUNT);

  randomSeed(analogRead(0));

  buttonState = false;

  currentEffectIndex = 0;

  delay(500);
}

void loop()
{
  
  if (isPressed())
  {
    currentEffectIndex++;

    if (currentEffectIndex==sizeof(effects) / sizeof(int))
    {
      currentEffectIndex = 0;
    }
  }
  


  effects[currentEffectIndex]();
}

bool isPressed()
{
  bool isPressButton = false;

  if (digitalRead(BUTTON_PIN)!=buttonState)
  {
    delay(50);
  }

  if (digitalRead(BUTTON_PIN)!=buttonState)
  {
    isPressButton = true;
    buttonState=!buttonState;
  }

  return isPressButton;
}

void shift()
{
  for(int i=LED_COUNT;i>0;i--)
  {
    strip[i] = strip[i-1];
  }
}

void effect1()
{
    // Включаем все светодиоды.
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Red; // Красный цвет.
  }
  // Передаем цвета ленте.
  FastLED.show();
  // Ждем 500 мс.
  delay(500);
  // Выключаем все светодиоды.
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Black; // Черный цвет, т.е. выключено.
  }
  // Передаем цвета ленте.
  FastLED.show();
  // Ждем 500 мс.
  delay(500);
}


void effect2()
{
    // Включаем все светодиоды.
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Red; // Красный цвет.
  }
  // Передаем цвета ленте.
  FastLED.show();
  // Ждем 500 мс.
  delay(500);
  // Выключаем все светодиоды.
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Black; // Черный цвет, т.е. выключено.
  }
  // Передаем цвета ленте.
  FastLED.show();
  // Ждем 500 мс.
  delay(500);
}
void effect3()
{
  
}