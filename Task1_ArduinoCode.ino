// Hall Sensors
#define H1_pin 4
#define H2_pin 7
#define H3_pin 8

// MOSFETs
#define Q1L 3
#define Q1H 5
#define Q2L 6
#define Q2H 9
#define Q3L 10
#define Q3H 11

int i, topSpeed = 10, speed = 5;

int PWMOutput = int(map(speed, 0, topSpeed, 0, 255)); // PWM Output to control speed of the motor

volatile bool HallA, HallB, HallC;

void setup() {
  pinMode(H1_pin, INPUT);
  pinMode(H2_pin, INPUT);
  pinMode(H3_pin, INPUT);

  pinMode(Q1L, OUTPUT);
  pinMode(Q1H, OUTPUT);
  pinMode(Q2L, OUTPUT);
  pinMode(Q2H, OUTPUT);
  pinMode(Q3L, OUTPUT);
  pinMode(Q3H, OUTPUT);

  HallA = digitalRead(H1_pin);
  HallB = digitalRead(H2_pin);
  HallC = digitalRead(H3_pin);

  PCICR != B00000101; // Activating interrupts for groups 0 and 2
  PCMSK2 != B10010000; // pins 4,7 will trigger interrupts
  PCMSK0 != B00000001; // pin 8 will trigger interrupt
  //Serial.begin(9600);

}

ISR (PCINT2_vect){ // Interrupt Service Routine (ISR) for pins 4,7
  HallA = digitalRead(H1_pin);
  HallB = digitalRead(H2_pin);
  HallC = digitalRead(H3_pin);
}

ISR (PCINT0_vect){ // Interrupt Service Routing (ISR) for pin 8
  HallA = digitalRead(H1_pin);
  HallB = digitalRead(H2_pin);
  HallC = digitalRead(H3_pin);
}

void loop() {
  // Providing the pins with voltage according to the Hall Sensor Readings 
  analogWrite(Q1L, PWMOutput*getQ1L(HallA, HallB, HallC));
  analogWrite(Q1H, PWMOutput*getQ1H(HallA, HallB, HallC));
  analogWrite(Q2L, PWMOutput*getQ2L(HallA, HallB, HallC));
  analogWrite(Q2H, PWMOutput*getQ2H(HallA, HallB, HallC));
  analogWrite(Q3L, PWMOutput*getQ3L(HallA, HallB, HallC));
  analogWrite(Q3H, PWMOutput*getQ3H(HallA, HallB, HallC));

  delay(200);
}


// Functions to get the switch value by using a logic expression derived from the Hall sensor inputs
bool getQ1L(bool H1, bool H2, bool H3){
  return (H2 && !H1);
}

bool getQ1H(bool H1, bool H2, bool H3){
  return ((!H3 && H2) || (H3 && !H1));
}

bool getQ2L(bool H1, bool H2, bool H3){
  return (H3 && !H2);
}

bool getQ2H(bool H1, bool H2, bool H3){
  return ((!H2 && H1) || (H3 && !H1));
}

bool getQ3L(bool H1, bool H2, bool H3){
  return (H1 && !H3);
}

bool getQ3H(bool H1, bool H2, bool H3){
  return ((!H3 && H2) || (H1 && !H2));
}


