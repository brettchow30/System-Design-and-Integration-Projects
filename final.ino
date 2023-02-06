#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal.h>

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define SS_PIN 53
#define RST_PIN 4
#define UP 16748655
#define DOWN 16769055
#define ONE 16724175
#define TWO 16718055
#define THREE 16743045
#define waterPin A1
#define tempPin A3

Servo servo;
MFRC522 mfrc522(SS_PIN, RST_PIN);
IRrecv irrecv(5);
decode_results results;
LiquidCrystal lcd(48,46,38,42,27,26);

const int forward=7;
//const int backward=7;
int time_delay = 3000;
int val = 0;
int servoPin = 23;
const int buzzer = 8;
long duration;
int distance, displayMotorSpeed;
bool access = false;
float motorSpeed, maxSpeed = 0.0;
int Headlight1 = 10;
int Headlight2 = 12;
int lightStatus = 1;
int Vo, coolantLevel;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int irValue;
const byte irPin = 5;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(forward, OUTPUT);
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  servo.attach(servoPin);
  servo.write(0);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(buzzer, OUTPUT);
  irrecv.enableIRIn(); // enable IR remote
  //IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);
  pinMode(Headlight1, OUTPUT);
  pinMode(Headlight2, OUTPUT);
  lcd.begin(16,2);
}
void loop() {
  if (access == false){
    access = RFID();
  }
  else{
    
    /// ULTRASONIC SENSOR SETUP ///
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    if (distance > 30){
        distance = 30;   
      }
      if (distance >= 15){
        servo.write(180);
      }
      else if (distance < 15){
        servo.write(0);
      }
    Serial.print(distance);
    Serial.print(" ");

    
    /// IR REMOTE SETUP ///
    if (irrecv.decode(&results)){
      //Serial.println(results.value);
      if (results.value == UP){
        maxSpeed = motorSpeed + 25.5;
        if (maxSpeed > 255){
          maxSpeed = 255;
        }
      }
      if (results.value == DOWN){
        maxSpeed = motorSpeed - 25.5;
        if (maxSpeed <= 0){
          maxSpeed = 0;
        }
      }
      if (results.value == ONE){
        analogWrite(Headlight1, 0);
        analogWrite(Headlight2, 0);
        lightStatus = 1;
      }
      if (results.value == TWO){
        analogWrite(Headlight1, 80);
        analogWrite(Headlight2, 80);
        lightStatus = 2;
        
      }
      if (results.value == THREE){
        analogWrite(Headlight1, 255);
        analogWrite(Headlight2, 255);
        lightStatus = 3;
      }
    irrecv.resume();
  }
    motorSpeed = map(distance,0,30,0,maxSpeed);
    displayMotorSpeed = map(maxSpeed,0,255,0,100);
//    Serial.print(motorSpeed);
//    Serial.print(" ");

    Serial.print(lightStatus);
    Serial.print(" ");
    Serial.print(displayMotorSpeed);
    Serial.print(" ");
    


    /// WATER COOLANT AND TEMPERATURE SETUP ///
    coolantLevel = analogRead(waterPin);
    Vo = analogRead(tempPin);
    R2 = R1 * (1023.0 / (float)Vo - 1.0);
    logR2 = log(R2);
    T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
    Tc = T - 273.15;
    Tf = (Tc * 9.0)/ 5.0 + 32.0; 
    if (coolantLevel <= 300 || Tf >= 90){
      motorSpeed = 0;
      analogWrite(Headlight1, 0);
      analogWrite(Headlight2, 0);
      lightStatus = 1;
      servo.write(0);
    }
    
    Serial.print(coolantLevel);
    Serial.print(" ");
     
    Serial.print(Tf);
    Serial.println(" ");

    analogWrite(forward, motorSpeed);
    displaylcd();
    delay(300);
  }
}

/// LCD SETUP//
void displaylcd(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write("Speed: ");
  lcd.setCursor(7,0);
  lcd.print(displayMotorSpeed);
  lcd.setCursor(10,0);
  lcd.write("%");
  
  lcd.setCursor(0,1);
  lcd.write("CL:");
  lcd.setCursor(3,1);
  lcd.print(coolantLevel);
  
  lcd.setCursor(7,1);
  lcd.write("Tmp:");
  lcd.setCursor(11,1);
  lcd.print(Tf);
  lcd.setCursor(15,1);
  lcd.write("F");  
}


/// RFID SETUP ///
int RFID(){
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return 0;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return 0;
  }
  //Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  if (content.substring(1) == "13 A4 2B 16") //change here the UID of the card/cards that you want to give access
  {
    tone(buzzer, 5000); // Send 5KHz sound signal...
    delay(500);        // ...for .5 sec
    noTone(buzzer);     // Stop sound...
    delay(500);        // ...for .5 sec
    return true;
  }
 
 else   {
    tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(500);        // ...for .5 sec
    noTone(buzzer);     // Stop sound...
    delay(500);        // ...for .5 sec
    return false;
  }
}
