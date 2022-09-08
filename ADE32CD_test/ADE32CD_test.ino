// ADE32CD_test.ino
// 2022.07.20 New Create by B.T.O
// 2022.08.09 Suppress DAC Noise
//
#include <Ticker.h>
#include <FastLED.h>
#include "ongen16k.h"

#define PIN_I2S_SCK  26     // 14
#define PIN_I2S_FS   25     // 25
#define PIN_I2S_SD   21     // 26

#define LED_PIN     4
#define NUM_LEDS    4
#define BRIGHTNESS  40
#define LED_TYPE    SK6812
#define COLOR_ORDER GRB
#define DELAYVAL    750

#define LED_BUILTIN 13          // bultin LED

float timer_interval = 0.1;  //seconds
Ticker Timer1;
volatile uint8_t counter = 1;

CRGB leds[NUM_LEDS];

static uint8_t   rgb_table[5][3] = {
     {0,          0,          0         },
     {BRIGHTNESS, 0,          0         },
     {0,          BRIGHTNESS, 0         },
     {0,          0,          BRIGHTNESS},
     {BRIGHTNESS, BRIGHTNESS, BRIGHTNESS},
};

#include <I2S.h>
const int amplitude = 500; // amplitude of square wave
const int sampleRate = 16000; // sample rate in Hz
const int bps = 16;

short sample = amplitude; // current sample value

i2s_mode_t mode = I2S_PHILIPS_MODE; // I2S decoder is needed

//==================================================================
// led_test() : Test NeoPixels 
//==================================================================
static void led_test()
{
  for (int i = 0; i < 5; i++) {
    FastLED.clear();  // clear all pixel data
    for(int j = 0; j < NUM_LEDS; j++) { // For each pixel...
      leds[j] = CRGB(rgb_table[i][0],rgb_table[i][1],rgb_table[i][2]);
    }
    FastLED.show();
    delay(DELAYVAL); // Pause before next pass through loop
  }
}

//==================================================================
// led_test2() : Test NeoPixels 
//==================================================================
void led_test2()
{
  FastLED.clear();  // clear all pixel data
  for(int j = 0; j < NUM_LEDS; j++) { // For each pixel...
    leds[j] = CRGB(rgb_table[counter][0],rgb_table[counter][1],rgb_table[counter][2]);
  }
  FastLED.show();
  counter++;
  if (counter > 4) counter = 1;
}

//==================================================================
// i2s_test() : Test I2S DAC & Speaker 
//==================================================================
static void i2s_test(void)
{
  // start I2S at the sample rate with 16-bits per sample
  if (!I2S.begin(mode, sampleRate, bps)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }
  I2S.setSckPin(PIN_I2S_SCK);
  I2S.setFsPin(PIN_I2S_FS);
  I2S.setDataPin(PIN_I2S_SD);

  for (int i = 0; i < sizeof(ongen); i += 2) {
      sample = pgm_read_word_near(ongen + i);
      //Serial.printf("%d : %04x\n");
      I2S.write(sample); // Right channel
      I2S.write(sample); // Left channel
  }
  I2S.end();
}

//==================================================================
// setup() : ESP32 Setup
//==================================================================
void setup()
{
  Serial.begin(115200);
  Serial.println("I2S simple tone");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);


  pinMode(PIN_I2S_SCK, OUTPUT);
  pinMode(PIN_I2S_FS, OUTPUT);
  pinMode(PIN_I2S_SD, OUTPUT);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(BRIGHTNESS);
}

//==================================================================
// loop() : ESP32 Main Loop
//==================================================================
void loop() {
  Serial.println("### Start LED Test");
  for (int i = 0; i < 3; i++) {
    led_test();
  }

  Serial.println("### Start I2S Test");

  Timer1.attach(timer_interval, led_test2);

  i2s_test();
  Serial.println("### End of Test");
//  delay(3000);
  Timer1.detach();
  FastLED.clear();  // clear all pixel data
  FastLED.show();
  while(1);                   // End of Test.
}
