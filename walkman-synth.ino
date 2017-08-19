//total lower bound seems to be 128-ish
#define LOWER_BOUND 200
#define UPPER_BOUND 220
#define OUTPUT_PIN 9
#define STATUS_LED_PIN 13
#define FREQUENCY 6

unsigned long periodMillis;

void setup() {
  // put your setup code here, to run once:
  TCCR1B = TCCR1B & 0b11111000 | 0b00000001; // set PWM freq to 32KHz
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  periodMillis = calculateWaveLength(FREQUENCY);
}

unsigned long calculateWaveLength(float frequency) {
  return (unsigned long)(1000.0 / frequency);
}

void loop() {
  // put your main code here, to run repeatedly:
  int outputValue = getOutputValue();
  if (outputValue > 128) {
    digitalWrite(STATUS_LED_PIN, HIGH);
  }
  else {
    digitalWrite(STATUS_LED_PIN, LOW);
  }
  analogWrite(OUTPUT_PIN, outputValue);
}

//int getOutputValue() {
//  unsigned long t = millis() % periodMillis;
//  if ((float)t / (float)periodMillis < 0.5) {
//    return UPPER_BOUND;
//  }
//  else {
//    return LOWER_BOUND;
//  }
//}
int getOutputValue() {
  unsigned long t = millis() % periodMillis;
  float r = 3.14*2.0 * (float)t / (float)periodMillis;
  float s = (sin(r) + 1.0)/2.0;
  float result = (float)LOWER_BOUND + (float)(UPPER_BOUND - LOWER_BOUND) * s;
  return (int)floor(result);
}
