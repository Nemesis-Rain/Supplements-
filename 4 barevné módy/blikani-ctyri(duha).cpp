#include <Arduino.h>
#include <SmartLeds.h>

const int LED_COUNT = 8;
const int DATA_PIN = 21;
const int CHANNEL = 0;

// SmartLed -> RMT driver (WS2812/WS2812B/SK6812/WS2813)
SmartLed leds( LED_WS2812, LED_COUNT, DATA_PIN, CHANNEL, DoubleBuffer );

// const int CLK_PIN = 23;
// APA102 -> SPI driver
//Apa102 leds(LED_COUNT, CLK_PIN, DATA_PIN, DoubleBuffer);

void SetLedAll(uint8_t R, uint8_t G, uint8_t B)
{
  for (int i = 0; i < LED_COUNT; i++)
    leds[i] = Rgb{R, G, B};
  leds.wait();
  leds.show();
}
// ------------------------------------------------------------------------------------------------



void setup() {
  Serial.begin(9600);  
}


void loop() {
for (int g = 0; g < 64; g++)
{
  SetLedAll(64, g, 0);
  delayMicroseconds(50000);
}

for (int g = 64; g >= 0; g--)
{
  SetLedAll(g, 64, 0);
  delayMicroseconds(50000);
}

for (int g = 0; g < 64; g++)
{
  SetLedAll(0, 64, g);
  delayMicroseconds(50000);
}

for (int g = 64; g >= 0; g--)
{
  SetLedAll(0, g, 64);
  delayMicroseconds(50000);
}

for (int g = 0; g < 64; g++)
{
  SetLedAll(g, 0, 64);
  delayMicroseconds(50000);
}

for (int g = 64; g >= 0; g--)
{
  SetLedAll(64, 0, g);
  delayMicroseconds(50000);
}

  leds.wait();  
  leds.show();
}


