int photo_pin = A0;
int vall = 0;
unsigned int val = 0;
float counter = 0;
void setup() {
  Serial.begin(9600);
  pinMode(6,OUTPUT);
  
}

void loop() {
  if (Serial.available() > 0){
    val = Serial.parseInt();
    analogWrite(6,counter);
    delay(500);
    vall = analogRead(photo_pin);
    Serial.println(vall);
    counter += 1;
  }

}
