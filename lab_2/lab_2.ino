                      int LED_PIN1 = 11;
                      int LED_PIN3 = 5;
                      int LED_PIN4 = 6;
                      int LED_PIN5 = 4;
                      
                      String value = "";
                      long randNumber;
                      String keys = "";
                      
                      int set1 = 0;
                      int set3 = 0;
                      int set4 = 0;
                      int set5 = 0;
                      int tries = 5;
                      void setup(){
                        Serial.begin(9600);
                        //set pins as outputs
                        pinMode(LED_PIN1, OUTPUT);
                        pinMode(LED_PIN3, OUTPUT);
                        pinMode(LED_PIN4, OUTPUT);
                        pinMode(LED_PIN5, OUTPUT);
                      
                        digitalWrite(LED_PIN1, LOW);
                        digitalWrite(LED_PIN3, LOW);
                        digitalWrite(LED_PIN4, LOW);
                        digitalWrite(LED_PIN5, LOW);
                      
                        randNumber = random(0, 9999);
                        keys = String(randNumber);
                        while(keys.length() != 4){
                          keys = "0"+keys;
                          }
                        Serial.print("The key is: ");
                        Serial.println(keys);
                        noInterrupts();//stop interrupts
                      
                        TCCR1A = 0;
                        TCCR1B = 0;
                        TIMSK1 = 0;
                        TCNT1 = 0;
                        OCR1A = 30000;
                        TCCR1B |= (1 << WGM12);
                        TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
                        TIMSK1 |= (1 << OCIE1A);
                      
                        TCCR3A = 0;
                        TCCR3B = 0;
                        TIMSK3 = 0;
                        TCNT3 = 0;
                        OCR3A = 30000;
                        TCCR3B |= (1 << WGM32);
                        TCCR3B |= (1 << CS32) | (0 << CS31) | (0 << CS30);
                        TIMSK3 |= (1 << OCIE3A);
                      
                        TCCR4A = 0;
                        TCCR4B = 0;
                        TIMSK4 = 0;
                        TCNT4 = 0;
                        OCR4A = 30000;
                        TCCR4B |= (1 << WGM42);
                        TCCR4B |= (1 << CS42) | (0 << CS41) | (0 << CS40);
                        TIMSK4 |= (1 << OCIE4A);
                      
                        TCCR5A = 0;
                        TCCR5B = 0;
                        TIMSK5 = 0;
                        TCNT5 = 0;
                        OCR5A = 30000;
                        TCCR5B |= (1 << WGM52);
                        TCCR5B |= (1 << CS52) | (0 << CS51) | (0 << CS50);
                        TIMSK5 |= (1 << OCIE5A);
                      
                      
                      interrupts();//allow interrupts
                      
                      }//end setup
                      
                      void loop(){
                        int flag = 0;
                        if (Serial.available() > 0) {
                            // read the incoming byte:
                            value = Serial.readString();
                            Serial.print("You have entered: ");
                            Serial.println(value);
                            if (tries > 0){
                              if (set1 == 0 && value[0] != keys[0]){
                                OCR1A = OCR1A - 5000;
                                flag = 1;
                              }
                            else{
                                set1 = 1;
                                digitalWrite(LED_PIN1, LOW);
                              }
                            if (set3 == 0 && value[1] != keys[1]){
                                OCR3A = OCR3A - 5000;
                                flag = 1;
                              }
                            else{
                                set3 = 1;
                                digitalWrite(LED_PIN3, LOW);
                              }
                            if (set4 == 0 && value[2] != keys[2]){
                                OCR4A = OCR4A - 5000;
                                flag = 1;
                              }
                            else{
                                set4 = 1;
                                digitalWrite(LED_PIN4, LOW);
                              }
                            if (set5 == 0 && value[3] != keys[3]){
                                OCR5A = OCR5A - 5000;
                                flag = 1;
                            }
                            else{
                                set5 = 1;
                                digitalWrite(LED_PIN5, LOW);
                              }
                             
                            if (flag == 1) {
                              tries = tries - 1;
                              if (tries <= 0){
                                  if (set1 == 0){
                                    OCR1A = 1;
                                    digitalWrite(LED_PIN1, HIGH);
                                    }
                                  if (set3 == 0){
                                    OCR3A = 1;
                                    digitalWrite(LED_PIN3, HIGH);
                                    }
                                  if (set4 == 0){
                                    OCR4A = 1;
                                    digitalWrite(LED_PIN4, HIGH);
                                    }
                                  if (set5 == 0){
                                    OCR5A = 1;
                                    digitalWrite(LED_PIN5, HIGH);
                                    }
                                }
                             
                              }
                           
                               
                              }
                        }
                      }
                      
                      
                      
                      ISR(TIMER1_COMPA_vect){//timer1 interrupt 8kHz toggles pin 9
                      //generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
                        if (set1 == 0){
                          digitalWrite(LED_PIN1, !(digitalRead(LED_PIN1)));
                          }
                        else{
                          digitalWrite(LED_PIN1, LOW);
                          }   
                       
                      }
                      
                      ISR(TIMER3_COMPA_vect){//timer1 interrupt 8kHz toggles pin 9
                      //generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
                        if (set3 == 0){
                          digitalWrite(LED_PIN3, !(digitalRead(LED_PIN3)));
                          }
                        else{
                          digitalWrite(LED_PIN3, LOW);
                          }  
                      }
                      
                      ISR(TIMER4_COMPA_vect){//timer1 interrupt 8kHz toggles pin 9
                      //generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
                      
                        if (set4 == 0){
                          digitalWrite(LED_PIN4, !(digitalRead(LED_PIN4)));
                          }
                        else{
                          digitalWrite(LED_PIN4, LOW);
                          }  
                      }
                      
                      ISR(TIMER5_COMPA_vect){//timer1 interrupt 8kHz toggles pin 9
                      //generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
                        if (set5 == 0){
                          digitalWrite(LED_PIN5, !(digitalRead(LED_PIN5)));
                          }
                        else{
                          digitalWrite(LED_PIN5, LOW);
                          }  
                      }
