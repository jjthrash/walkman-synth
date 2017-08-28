#include <MIDI.h>
#include <SoftwareSerial.h>


//total lower bound seems to be 128-ish
#define LOWER_BOUND 10000
#define UPPER_BOUND 20000
#define FREQUENCY 6

unsigned long periodMillis;

#define SOFT_SERIAL_RX_PIN 12
#define SOFT_SERIAL_TX_PIN 13

SoftwareSerial SoftSerial(SOFT_SERIAL_RX_PIN, SOFT_SERIAL_TX_PIN);

MIDI_CREATE_INSTANCE(SoftwareSerial, SoftSerial, MIDI);

#if 0
#define TCCR1A_VALUE 0b00100001
#define TCCR1B_VALUE 0b00010010
#define OCR1A_VALUE 20000
#define BASE_PWM 1313
#define PWM_0 7000
#define PWM_12 12700
#else
#define TCCR1A_VALUE 0b00100001
#define TCCR1B_VALUE 0b00010001
#define OCR1A_VALUE 268
#define BASE_PWM 65
#define PWM_0 135
#define PWM_12 215
#endif

byte note = 0;

void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{
  Serial.println("note on");
  note = inNote;
//    const bool firstNote = midiNotes.empty();
//    midiNotes.add(MidiNote(inNote, inVelocity));
//    handleNotesChanged(firstNote);
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
  Serial.println("note off");
  note = 0;
//    midiNotes.remove(inNote);
//    handleNotesChanged();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up");

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin();
  MIDI.turnThruOn();

  Serial.println("MIDI enabled");

  TCCR1A = TCCR1A_VALUE;
  TCCR1B = TCCR1B_VALUE;

  OCR1A = OCR1A_VALUE;  // 20ms. Cycle Time
  OCR1B = 0;  // 1.5 ms. Initial Pulse Width to center servo

  DDRB |= (1 << PB2);  // Output on PB2

  periodMillis = calculateWaveLength(FREQUENCY);
  Serial.println("setup complete");
}

unsigned long calculateWaveLength(float frequency) {
  return (unsigned long)(1000.0 / frequency);
}

void loop() {
  // put your main code here, to run repeatedly:
  int outputValue = getOutputValue();
  OCR1B = outputValue;
  MIDI.read();
//  if (millis() % 250 == 0) {
//    Serial.println(outputValue);
//  }
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
//int getOutputValue() {
//  unsigned long t = millis() % periodMillis;
//  float r = 3.14*2.0 * (float)t / (float)periodMillis;
//  float s = (sin(r) + 1.0)/2.0;
//  float result = (float)LOWER_BOUND + (float)(UPPER_BOUND - LOWER_BOUND) * s;
//  return (int)floor(result);
//}
int notes[] = {
  4,
  2,
  0,
  2,
  4,
  4,
  4,
  4,
  2,
  2,
  2,
  2,
  4,
  7,
  7,
  7
};

//int getOutputValue() {
//  unsigned long seconds = millis() / 1000L;
////  unsigned long result = 5000 + ((seconds%45L)*500L);
////  unsigned long result = noteToPWM(seconds%18);
//  int result = noteToPWM(notes[seconds%16]);
//  return (int)result;
//}

//int getOutputValue() {
//  int val = analogRead(0);
//  return (int)((float)val/3.8209);
//}


int noteToPWM(int note) {
  float scalar = (float)(PWM_12-PWM_0);
  return BASE_PWM+(int)(scalar*pow(2, (float)note/12.0));
}

int getOutputValue() {
  if (note >= 69 && note < 69+19) {
    return noteToPWM(note - 69);
  }
}

