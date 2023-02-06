
int led_red = 44;
int led_blue = 11;
int led_green = 6;
int led_yellow = 5;

String input = "";
long code;
String keys = "";

int set_red = 0;
int set_blue = 0;
int set_green = 0;
int set_yellow = 0;
int rounds = 0;

// the setup outine runs once when you press reset:
void setup() {
  pinMode(led_red, OUTPUT);
  pinMode(led_blue, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  
  digitalWrite(led_red, LOW);
  digitalWrite(led_blue, LOW);
  digitalWrite(led_green, LOW);
  digitalWrite(led_yellow, LOW);
  
  Serial.begin(9600);
  code = random(0,9999);
  keys = String(code);
  while(keys.length() != 4){
    keys = "0"+keys;
    }
  Serial.print("The safe code is: ");
  Serial.println(keys);
  
  noInterrupts();

  //led_red
  TCCR5A = 0;
  TCCR5B = 0;
  TIMSK5 = 0;
  TCNT5 = 0;
  OCR5A = 30000;
  TCCR5B |= (1 << WGM52);
  TCCR5B |= (1 << CS52) | (0 << CS51) | (0 << CS50);
  TIMSK5 |= (1 << OCIE5A);

  //led_blue
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 = 0;
  TCNT1 = 0;
  OCR1A = 30000;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
  TIMSK1 |= (1 << OCIE1A);

  //led_green
  TCCR4A = 0;
  TCCR4B = 0;
  TIMSK4 = 0;
  TCNT4 = 0;
  OCR4A = 30000;
  TCCR4B |= (1 << WGM42);
  TCCR4B |= (1 << CS42) | (0 << CS41) | (0 << CS40);
  TIMSK4 |= (1 << OCIE4A);

  //led_yellow
  TCCR3A = 0;
  TCCR3B = 0;
  TIMSK3 = 0;
  TCNT3 = 0;
  OCR3A = 30000;
  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << CS32) | (0 << CS31) | (0 << CS30);
  TIMSK3 |= (1 << OCIE3A);
  
interrupts(); //Allow interrupts
}

 // the loop routine runs over and over again forever:
void loop() {
  int flag = 0;
  if (Serial.available() > 0){
    input = Serial.readString();
    Serial.print("You have entered: ");
    Serial.println(input);
    if (input.length() == 4){
      if (rounds <= 5){
        if (set_red == 0 && input[0] != keys[0]){
          OCR5A = OCR5A - 5000;
          flag = 1;
        }
        else{
          set_red = 1;
          digitalWrite(led_red, LOW);
        }
        if (set_blue == 0 && input[1] != keys[1]){
          OCR1A = OCR1A - 5000;
          flag = 1;
        }
        else{
          set_blue = 1;
          digitalWrite(led_blue, LOW);
        }
        if (set_green == 0 && input[2] != keys[2]){
          OCR4A = OCR4A - 5000;
          flag = 1;
        }
        else{
          set_green = 1;
          digitalWrite(led_green, LOW);
        }
        if (set_yellow == 0 && input[3] != keys[3]){
          OCR3A = OCR3A - 5000;
          flag = 1;
        }
        else{
          set_yellow = 1;
          digitalWrite(led_yellow, LOW);
        }
    
        if (flag == 1){
          rounds = rounds + 1;
          if (rounds == 5){
            if (set_red == 0){
              OCR5A = 1;
              digitalWrite(led_red, HIGH);
            }
            if (set_blue == 0){
              OCR1A = 1;
              digitalWrite(led_blue, HIGH);
            }
            if (set_green == 0){
              OCR4A = 1;
              digitalWrite(led_green, HIGH);
            }
            if (set_yellow == 0){
              OCR3A = 1;
              digitalWrite(led_yellow, HIGH);
            }
            Serial.println("You failed!");
            delay(100);
            exit(0);
          }
        }
      else{
        Serial.println("You unlocked the safe!");
        delay(100);
        exit(0);
      }
      }
}
else{
  Serial.println("The safe combination must be a 4-digit number. Please try again.");
}
}
}

ISR(TIMER5_COMPA_vect){
  if (set_red == 0){
    digitalWrite(led_red, !(digitalRead(led_red)));
  }
  else{
    digitalWrite(led_red, LOW);
  }
}

ISR(TIMER1_COMPA_vect){
  if (set_blue == 0){
    digitalWrite(led_blue, !(digitalRead(led_blue)));
  }
  else{
    digitalWrite(led_blue, LOW);
  }
}

ISR(TIMER4_COMPA_vect){
  if (set_green == 0){
    digitalWrite(led_green, !(digitalRead(led_green)));
  }
  else{
    digitalWrite(led_green, LOW);
  }
}

ISR(TIMER3_COMPA_vect){
  if (set_yellow == 0){
    digitalWrite(led_yellow, !(digitalRead(led_yellow)));
  }
  else{
    digitalWrite(led_yellow, LOW);
  }
}
