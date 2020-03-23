#include <Arduino.h>
#include "FastLED.h"
// Подключаем библиотеку FastLED.

typedef void (* GenericFP)(); //function pointer prototype to a function which takes an 'int' an returns 'void'

void shift();
void effect1();
void effect2();
void effect3();
bool isPressed();
void keyPressed();
void debounceTime();

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
int debounceTimeCounter;

 
void setup()
{

  delay(3000);

  // Добавляем ленту
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(strip, LED_COUNT);

  randomSeed(analogRead(0));

  buttonState = false;

  currentEffectIndex = 0;
  debounceTimeCounter = 0;


  attachInterrupt(BUTTON_PIN, keyPressed, HIGH);

  Serial.begin(115200);

}

void keyPressed()
{
  Serial.println("pressed");
  //timer1_attachInterrupt(debounceTime);
  //timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);

}

// void debounceTime()
// {
//   debounceTimeCounter++;

//   if (debounceTimeCounter==10000)
//   {
//     debounceTimeCounter=0;
//     timer1_disable();

//     Serial.println("debunced");
//   }

// }

void loop()
{
  
  // if (isPressed())
  // {
  //   currentEffectIndex++;

  //   if (currentEffectIndex==sizeof(effects) / sizeof(int))
  //   {
  //     currentEffectIndex = 0;
  //   }
  // }
  


  effects[currentEffectIndex]();
}

// bool isPressed()
// {
//   bool isPressButton = false;

//   if (digitalRead(BUTTON_PIN)!=buttonState)
//   {
//     delay(50);
//   }

//   if (digitalRead(BUTTON_PIN)!=buttonState)
//   {
//     isPressButton = true;
//     buttonState=!buttonState;
//   }

//   return isPressButton;
// }

void shift()
{
  for(int i=LED_COUNT;i>0;i--)
  {
    strip[i] = strip[i-1];
  }
}

void effect1()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Black; // Красный цвет.
  }

  FastLED.show();

  int i=0;


  while(i<LED_COUNT)
  {
    int position = random16(LED_COUNT);

    if (strip[position])
    {
      continue;
    }

    strip[position]=CRGB(random8(255),random8(255),random8(255));
    i++;
    FastLED.show();
    delay(50);
  }

  // Ждем 500 мс.
  delay(500);
  // Выключаем все светодиоды.
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Black; // Черный цвет, т.е. выключено.
  }
  // Передаем цвета ленте.
  FastLED.show();
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
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip[i] = CRGB::Black; // Черный цвет, т.е. выключено.
  }
  // Передаем цвета ленте.
  FastLED.show();
  // Ждем 500 мс.
  delay(500);
}