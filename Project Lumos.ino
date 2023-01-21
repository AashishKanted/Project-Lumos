/*************************************************************
Project Lumos

Jan 7th 2022

Code to run LED strip and a servo by sending commands over the
internet.

https://github.com/AashishKanted

Github repository :https://github.com/AashishKanted/Project-Lumos

Installing the necessary libraries-

1. ESP8266 library
* Install the current upstream Arduino IDE at the 1.8.9 level or later. The current version is on the Arduino website.
* Start Arduino and open the Preferences window.
* Enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into the File>Preferences>Additional Boards Manager URLs field of the Arduino IDE, and press okay
* Open Boards Manager from Tools > Board menu and install esp8266 platform (and don't forget to select your ESP8266 board from Tools > Board menu after installation).

2.BLYNK
*Open Manage Libraries from Tools, search for Blynk and install

3.FastLED
*Open Manage Libraries from Tools, search for FastLED and install

*************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPL6K9ALuI0" 
#define BLYNK_DEVICE_NAME           "Sheesh ESP8266"
#define BLYNK_AUTH_TOKEN            "91VKc6v39hzNrrdTlFVf4u6in4Mu_t7a"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


//Libraries to include
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "FastLED.h"
#include <Servo.h>

// Definitions
#define NUM_LEDS 120
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define FORWARD 0
#define BACKWARD 1
#define SLOW 250
#define MEDIUM 50
#define FAST 5
int dumpling[] = {1,0,0,1,0,0,1, 1,0,0,1,1,0,0, 1,1,0,1,1,1,1, 1,1,1,0,1,1,0, 1,1,0,0,1,0,1, 1,0,1,1,0,0,1, 1,1,0,1,1,1,1, 1,1,1,0,1,0,1};


Servo servo;
CRGB leds[NUM_LEDS];
#define PIN1 5
int data =255; // brightness data
//int r,g,b;
//intitial colour
int r = 5;
int g = 5;
int b = 5;
int speed = 10;
int direction = 0; //0-forward, 1-backward
int servo_rest_angle = 90;  //angle servo rests in after turning on or off

uint16_t colorIndex[NUM_LEDS];

// for colour graadient
DEFINE_GRADIENT_PALETTE( greenblue_gp ) { 
  0,   0,  255, 245,
  46,  0,  21,  255,
  179, 12, 250, 0,
  255, 0,  255, 245
};

CRGBPalette16 greenblue = greenblue_gp;

char auth[] = BLYNK_AUTH_TOKEN;


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Aashish's pixel";
char pass[] = "9840530003";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
  Serial.println(value);
  
  // turning on and off code TO BE ADDED
  
}

BLYNK_WRITE(V4)
{
  // Zebra Colour picker
  // Set incoming value from pin V4 to a variable
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();

  // Update state
  Serial.print("Colour value(rgb): ");
  Serial.print(r);
  Serial.print(g);
  Serial.print(b);
  Serial.println(" ");
  static1(r, g, b);
}

BLYNK_WRITE(V5)
{
  // brightness
  data = param[0].asInt();
  Serial.println(data);
  // Update brightness
  FastLED.setBrightness(data);
  FastLED.show();
}

BLYNK_WRITE(V7)
{
  // Speed of animations
  speed = 255 - param[0].asInt();
  Serial.println(speed);
}

BLYNK_WRITE(V8)
{
  // direction of animations
  direction = param.asInt();
  Serial.println(direction);
}

BLYNK_WRITE(V9)
{
  // servo motor control
  int angle = param.asInt();
  servo.attach(4); //D2
  delay(200);
  Serial.println("servo turning");
  servo.write(angle);
  delay(700);
  servo.write(servo_rest_angle);
  delay(700);
  servo.detach();
}


// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
//  // Change Web Link Button message to "Congratulations!"
//  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
//  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
//  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  FastLED.addLeds<LED_TYPE, PIN1, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(15, 15, 15);
  }
  FastLED.show();

  //connecting to wifi
  Blynk.begin(auth, ssid, pass);
  
  //led show message
  for (int i = 0; i < (sizeof(dumpling)/sizeof(int)); i++)
  {
    leds[i] = CRGB(15*dumpling[i], 55*dumpling[i], 15*dumpling[i]);
  } 
  for (int i = (sizeof(dumpling)/sizeof(int));i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();

  
  /* if a timer is needed uncomment
   // Setup a function to be called every second
   timer.setInterval(1000L, myTimerEvent);
  */
}

void loop()
{
  Blynk.run();
  timer.run();
  
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  
}

//static single colour
void static1(int r, int g, int b){
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
}

//colorwipe
void colorWipe(CRGB c, int speed, int direction){
  for(int i=0; i<NUM_LEDS; i++){
    if(direction == 0){
      leds[i] = c;
    }
    else{
      leds[NUM_LEDS-1-i] = c;
    }
    FastLED.show();
    delay(speed);
    }
}

// Rainbow colors that slowly cycle across LEDs
void rainbow(int cycles, int speed){ // TODO direction
  if(cycles == 0){
    for(int i=0; i< NUM_LEDS; i++) {
      leds[i] = Wheel(((i * 256 / NUM_LEDS)) & 255);
    }
    FastLED.show();
  }
  else{
    for(int j=0; j<256*cycles; j++) {
      for(int i=0; i< NUM_LEDS; i++) {
        leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      }
      FastLED.show();
      delay(speed);
    }
  }
}

// Theater-style crawling lights
void theaterChase(CRGB c, int cycles, int speed){

  for (int j=0; j<cycles+2; j++) {  
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        int pos = i+q;
        leds[pos] = c;    //turn every third pixel on
      }
      FastLED.show();

      delay(speed+100);

      for (int i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q] = CRGB::Black;        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// Theater-style crawling lights
void gradient(){
  for (int j = 0; j<1000000; j++){
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(greenblue, colorIndex[i]);
  }
  
  EVERY_N_MILLISECONDS(7){
    for (int i = 0; i < NUM_LEDS; i++) {
      colorIndex[i]++;
    }
  }
  FastLED.show();
}}

BLYNK_WRITE(V6)
{
  int preset_no = param.asInt();
  if (preset_no == 1){
    static1(r,g,b);
    }
  else if (preset_no == 2){
    colorWipe(CRGB(r,g,b),speed,direction);
    }
  else if (preset_no == 3){
    rainbow(1,speed);
    }
  else if (preset_no == 4){
    theaterChase(CRGB(r,g,b),1,speed);
  }
  //add new presets here
  else if (preset_no == 5){
    gradient();
  }
  else if (preset_no == 6){
    theaterChase(CRGB(r,g,b),1,speed);
  }
}
