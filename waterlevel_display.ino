// Include Wire Library for I2C
#include <Wire.h>

// Include Adafruit Graphics & OLED libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Reset pin not used but needed for library
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int trigPin = 5;
int echoPin = 7;
int supply = 8; // relay IN pin
int maxLED = 9; //red
int minLED = 10; //green
int supplyLED = 11; //orange

int depth = 20;

void setup() {
  // initialize OLED with I2C addr 0x3C
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Serial.begin(9600);
  // Debug console
  delay(10);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);   
  pinMode(minLED, OUTPUT);
  pinMode(maxLED, OUTPUT);
  pinMode(supply, OUTPUT);
  pinMode(supplyLED, OUTPUT);
}

void sendSensor(){
  // Delay to allow sensor to stabalize
  delay(1000);

  long duration, distance, level, percentage;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration=pulseIn(echoPin, HIGH);
  distance = duration * 0.034 /2;
  level = depth - distance;
  percentage = map(level,0,depth,0,100); // map(val, fromLow, fromHigh, toLow, toHigh)
  Serial.println(percentage);
  delay(10);

  // Clear the display
  display.clearDisplay();
  //Set the color 
  display.setTextColor(WHITE);
  //Set the font size
  display.setTextSize(1);
  //Set the cursor coordinates
  display.setCursor(0,0);
  display.print("Smart Watertank");
  display.setCursor(0,10); 
  display.print("Level:    "); 
  display.print(level);
  display.print("cm");
  display.setCursor(0,20); 
  display.print("Percentage:    "); 
  display.print(percentage);
  display.print("%");

  if(level>=15) 
  {
    digitalWrite(maxLED, HIGH);
  }
  else if(level < 15)
  {
    digitalWrite(maxLED, LOW);
  }
  if(level<=5)
  {
     digitalWrite(minLED, HIGH);
  }
  else if(level > 5)
  {
    digitalWrite(minLED, LOW);
  }
  if(level<=5)
  {
    digitalWrite(supply, LOW); // relay on
    digitalWrite(supplyLED, HIGH);
  }
  else if(level>=15)
  {
    digitalWrite(supply, HIGH); // relay off
    digitalWrite(supplyLED, LOW);
  }

}

void loop() {
  sendSensor();
  display.display();

}
