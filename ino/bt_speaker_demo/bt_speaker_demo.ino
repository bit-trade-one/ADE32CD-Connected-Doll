// Connected Doll Bluetooth Speaker sample
// Ver 0.1
//
#include "BluetoothA2DPSink.h"
#include <FastLED.h>
#define LED_PIN     4           // io4 : NeoPixels
#define NUM_LEDS    4
#define BRIGHTNESS  30
#define LED_TYPE    SK6812
#define COLOR_ORDER GRB
#define LED_BUILTIN 13          // bultin LED
#define LED_DELAY   5
CRGB leds[NUM_LEDS];

BluetoothA2DPSink a2dp_sink;

//==========================================
//  avrc_metadata_callback :
//==========================================
static void avrc_metadata_callback(uint8_t id, const uint8_t* msg) {
    switch (id) {
      case 0x01:
        Serial.printf("=======================================================\n");
        Serial.printf("  Music : %s\n", msg);
        break;
      case 0x02:
        Serial.printf("  Artist: %s\n", msg);
        break;
      case 0x04:
        Serial.printf("  Album : %s\n", msg);
        break;
      default:
        break;
    }
}

//==========================================
//  led_stanby() :  stanby color
//==========================================
static void led_stanby()
{
    uint16_t i;
    for (i = 0; i <NUM_LEDS; i ++) {
        leds[i] = CRGB(255, 0, 0);
    }
    FastLED.show();
}

//==========================================
//  on_data_receive_callback : 
//==========================================
static void on_data_receive_callback(void) {
    static uint16_t count = 0, step = 0;
    uint16_t i;

    FastLED.clear();  // clear all pixel data

    for (i = 0; i <NUM_LEDS; i ++) {
        if (i == step) {
            leds[i] = CRGB(0, 0, 255);
        } else {
            leds[i] = CRGB(255, 255, 255);
        }
    }
    FastLED.show();
    count++;
    count %= LED_DELAY;
    if (count == 0) {
        step++;
        step = step % 4;
    } 
}

//==========================================
//  setup() : Arduino setup  
//==========================================
void setup() {
    Serial.begin(115200);
    // set builtin LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    // set NeoPixels to default color
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();  // clear all pixel data
    led_stanby();
    // set i2s pin 
    i2s_pin_config_t my_pin_config = {
        .bck_io_num = 26,
        .ws_io_num = 25,
        .data_out_num = 21,
        .data_in_num = I2S_PIN_NO_CHANGE
    };
    // set i2s mode
    static i2s_config_t i2s_config = {
        .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100, // updated automatically by A2DP
        .bits_per_sample = (i2s_bits_per_sample_t)32,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t) (I2S_COMM_FORMAT_STAND_I2S),
        .intr_alloc_flags = 0, // default interrupt priority
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = true,
        .tx_desc_auto_clear = true // avoiding noise in case of data unavailability
    };
    a2dp_sink.set_pin_config(my_pin_config);
    a2dp_sink.set_i2s_config(i2s_config);
    // set callbacks
    a2dp_sink.set_on_data_received(on_data_receive_callback);
    a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);
    // set device name
    a2dp_sink.start("BTO_Music");
}
//==========================================
//  main() : Arduino main
//==========================================
void loop() {
}
