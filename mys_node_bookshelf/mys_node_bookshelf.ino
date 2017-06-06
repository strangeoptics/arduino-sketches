#define MY_RADIO_NRF24
#define MY_DEBUG
#define MY_RF24_CE_PIN 8
#define MY_NODE_ID 46

#include <MySensors.h>
#include <SPI.h>
#include<FastLED.h>
#define NUM_LEDS 65
#define SENSOR_ID_RGB 22

CRGBArray<NUM_LEDS> leds;

int hue = 0;
int val = 0;
int RGB_values[3] = {0,0,0};

MyMessage msgPresent(SENSOR_ID_RGB, V_RGB);

void setup() { 

  FastLED.addLeds<NEOPIXEL,7>(leds, NUM_LEDS); 

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  FastLED.show(); 

  send(msgPresent.set("ff00ff"));

  request(SENSOR_ID_RGB, V_RGB);
}

void presentation()
{
    sendSketchInfo("BookShelfRGB", "0.1");

    present(SENSOR_ID_RGB, S_RGB_LIGHT);
}

void loop(){    
  
}

void receive(const MyMessage &message) {
  Serial.println("receive()");
  Serial.println(message.type);
  Serial.println(message.getString());

  String rgbHexString = message.getString();                      //Load the hex color code into a string
  long number = (long) strtol( &rgbHexString[0], NULL, 16);
  RGB_values[0] = number >> 16;
  RGB_values[1] = number >> 8 & 0xFF;
  RGB_values[2] = number & 0xFF;
  setColor(RGB_values[0],RGB_values[1],RGB_values[2]);
  
  
}

void setColor(int red, int green, int blue){
  Serial.print(red);
  Serial.print(",");
  Serial.print(green);
  Serial.print(",");
  Serial.println(blue);

  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = CRGB(red,green,blue);
  } 
  FastLED.show(); 
}
/*void change() {
  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = CHSV(hue255,255,val255);
  } 
  FastLED.show(); 
}*/

