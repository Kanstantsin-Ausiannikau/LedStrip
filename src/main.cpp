#include <Arduino.h>
#include "FastLED.h"
// Подключаем библиотеку FastLED.

typedef void (* GenericFP)(); //function pointer prototype to a function which takes an 'int' an returns 'void'

void shift();
void effect1();
void effect2();
void effect3();
void keyPressed();
void debounceTime();
void rainbowCycle(int SpeedDelay);
void setPixel(int Pixel, byte red, byte green, byte blue);
void showStrip();
byte * Wheel(byte WheelPos);
void setPixelHeatColor (int Pixel, byte temperature);
void Fire(int Cooling, int Sparking, int SpeedDelay);
void BouncingBalls(byte red, byte green, byte blue, int BallCount);
void setAll(byte red, byte green, byte blue);

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

bool doBreaked;


long startTimer;

 
void setup()
{
  delay(3000);

  // Добавляем ленту
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(strip, LED_COUNT);

  randomSeed(analogRead(0));

  buttonState = false;

  currentEffectIndex = 0;
  debounceTimeCounter = 0;

  doBreaked = false;


  Serial.begin(115200);

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
   if (digitalRead(BUTTON_PIN)==HIGH)
   {
     Serial.println(millis() - startTimer);
     Serial.println("pressed");
     doBreaked = true;

     timer1_disable();
   }
 }

void loop()
{
   if (doBreaked)
   {
     currentEffectIndex++;

     if (currentEffectIndex==sizeof(effects) / sizeof(int))
     {
       currentEffectIndex = 0;
    }
    doBreaked = false;
   }
  
  effects[currentEffectIndex]();
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
  rainbowCycle(20);  
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< LED_COUNT; i++) {

      if (doBreaked)
      {
        for (int i = 0; i < LED_COUNT; i++)
        {
          strip[i] = CRGB::Black; // Черный цвет, т.е. выключено.
        }
        // Передаем цвета ленте.
        FastLED.show();
        return;
      }

      c=Wheel(((i * 256 / LED_COUNT) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) 
{
   strip[Pixel].r = red;
   strip[Pixel].g = green;
   strip[Pixel].b = blue;
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < LED_COUNT; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

byte * Wheel(byte WheelPos) 
{
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

void showStrip()
{
  FastLED.show();
}

void effect2()
{
  Fire(55,120,15);
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[LED_COUNT];
  int cooldown;
 
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < LED_COUNT; i++) {
    cooldown = random(0, ((Cooling * 10) / LED_COUNT) + 2);
   
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= LED_COUNT - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
   
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < LED_COUNT; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void effect3()
{
  BouncingBalls(0xff,0,0, 3);
}

void BouncingBalls(byte red, byte green, byte blue, int BallCount) {
  float Gravity = -9.81;
  int StartHeight = 1;
 
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
 
  for (int i = 0 ; i < BallCount ; i++) {  
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
 
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
 
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (LED_COUNT - 1) / StartHeight);
    }
 
    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i],red,green,blue);
    }
   
    showStrip();
    setAll(0,0,0);
  }
}