#include <Arduino.h>
#include <Pixy2.h>
#include <Pixy2CCC.h>
#include <FastLED.h>

#define LED_PIN 7
#define COLOR_ORDER GRB
#define CHIPSET WS2811
#define NUM_LEDS 8

#define BRIGHTNESS 80
#define FRAMES_PER_SECOND 60

CRGB leds[NUM_LEDS];

Pixy2 pixy;

void setup()
{
  delay(1000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(115200);
  Serial.print("Starting...\n");

  pixy.init();
}

void loop()
{
  pixy.ccc.getBlocks();
  char buf[32];
  uint8_t xDirection;

  if (pixy.ccc.numBlocks)
  {
    xDirection = map(pixy.ccc.blocks[0].m_x, 0, 315, 0, NUM_LEDS);
    sprintf(buf, "X >>> %d:\n", xDirection);
    Serial.println(buf);
    fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
    leds[xDirection] = CHSV(255, 255, BRIGHTNESS);
    FastLED.show();
    FastLED.delay(1000 / FRAMES_PER_SECOND);
  }
}
