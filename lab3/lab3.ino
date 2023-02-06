int led = 6;
float temp = 0;
float voltage;
float duty_cycle;
void setup() {
  TCCR4A = 0b10000010;
  TCCR4B = 0b00010011;
  ICR4 = 12500;
  OCR4A = 5000;
  TCNT4 = 0;
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(A0, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  voltage = analogRead(A0); // read in voltage
  temp = voltage/1023.0;
  OCR4A = temp;
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" --> ");
  duty_cycle = ((float OCR4A) / (float ICR4));
  Serial.print("Duty Cycle: ");
  Serial.println(duty_cycle);
  //Serial.print(" --> ");
  delay(200);
}

ISR(TIMER4_COMPA_vect){
  
}
