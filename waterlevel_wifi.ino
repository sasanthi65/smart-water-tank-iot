/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID           "TMPL6vTR0pKkr"
#define BLYNK_TEMPLATE_NAME         "Smart Tank Project"
#define BLYNK_AUTH_TOKEN            "UKIzwFrSuuLBLIYEM1WCQE9Jf6GrGpwW"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Dialog 4G 442";
char pass[] = "5EcAb0d1";

// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 38400

ESP8266 wifi(&EspSerial);
BlynkTimer timer;

// define pins used
int trigPin = 5;
int echoPin = 7;
int supply = 8; // relay IN pin
int maxLED = 9; /red
int minLED = 10; //green
int supplyLED = 11; //orange

int depth = 20;

void setup() {
  // Debug console
  Serial.begin(115200);

  // Set ESP8266 baud rate
  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass, "blynk.cloud", 80);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

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

  // define variables
  long duration, distance, level, percentage;
  //clears the trigpin
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  //sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //reads the echoPin, return the sound wave travel time in microseconds
  duration=pulseIn(echoPin, HIGH);
  //calculating the distance
  distance = duration * 0.034 / 2;
  //calculating the water level filled at the moment
  level = depth - distance;
  //mapping the percentage of the water level filled 
  percentage = map(level,0,depth,0,100);
  Serial.println(percentage);
  delay(10);

  // green LED to indicate maximum level
  if(level>=15) 
  {
    digitalWrite(maxLED, HIGH);
  }
  else if(level < 15)
  {
    digitalWrite(maxLED, LOW);
  }

  // red LED to indicate minimum level
  if(level<=5)
  {
     digitalWrite(minLED, HIGH);
  }
  else if(level > 5)
  {
    digitalWrite(minLED, LOW);
  }

  // send signal to relay module
  if(level<=5)
  {
    digitalWrite(supply, LOW); //relay on
    digitalWrite(supplyLED, HIGH);
  }
  else if(level >= 15)
  {
    digitalWrite(supply, HIGH); //relay off
    digitalWrite(supplyLED, LOW);
  }

  Blynk.virtualWrite(V1, level);
  Blynk.virtualWrite(V2, percentage);
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}

