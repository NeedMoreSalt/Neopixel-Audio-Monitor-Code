#include <Adafruit_GFX.h>   // Core graphics library
unsigned long t;
#include <Adafruit_NeoPixel.h>

byte last[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#define RED_PEAK 5

#define LED_PIN    2

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 256
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

String sep = ":";

void setup() {
  Serial.begin(57600);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(5);
}

void loop() {
  while (Serial.available() > 0) {
    t = millis();

    String in = Serial.readStringUntil('\n');

    int f = in.substring(0, in.indexOf(sep)).toInt();
    int ff = in.substring(in.indexOf(sep) + sep.length()).toInt();

    if (f < 31 && ff < 16) {
      int x0 = f;
      uint32_t color;

      if (ff > last[f]) {
        for (int y = last[f]; y <= ff; y++) {
          int x = 31 - x0;
          switch ((int)x % 4) {
            case 0:
              color = strip.Color(255, 0, 0);
              break;
            case 1:
              color = strip.Color(0, 255, 0);
              break;
            case 2:
              color = strip.Color(0, 0, 255);
              break;
            case 3:
              color = strip.Color(255, 0, 255);
              break;

            default:
              color = strip.Color(255, 255, 255);
              break;
          }

          drawPixel(x, y - 1, color);
          strip.show();

        }
      } else if (last[f] > ff) {
        color = strip.Color(0, 0, 0);

        for (int y = last[f]; y > ff; y--) {
          drawPixel(31 - x0, y - 1, color);
          strip.show();

        }
      }

      last[f] = ff;
    }
  }
  if (millis() - t > 100) {
    wipe();
  }
}

void drawPixel(int x, int y, uint32_t color) {
  y = y / 2;
  if (x % 2 == 0) {
    strip.setPixelColor((x - 1) * 8 + y, color);
  }
  else {
    strip.setPixelColor((x - 1) * 8 + (8 - y)-1, color);

  }

}
void wipe() {
  for (int y = 7; y >= 0; y--) {
    for (int x = 1; x <= 32; x++) {
      drawPixel(x, y * 2, strip.Color(0, 0, 0));
    }
    strip.show();
    delay(50);
  }
}
