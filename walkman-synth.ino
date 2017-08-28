#include <MIDI.h>
#include <SoftwareSerial.h>

#define SOFT_SERIAL_RX_PIN 12
#define SOFT_SERIAL_TX_PIN 13

SoftwareSerial SoftSerial(SOFT_SERIAL_RX_PIN, SOFT_SERIAL_TX_PIN);

MIDI_CREATE_INSTANCE(SoftwareSerial, SoftSerial, MIDI);

#if 0
// 50Hz, TOP 20000
#define TCCR1A_VALUE 0b00100001
#define TCCR1B_VALUE 0b00010010
#define OCR1A_VALUE 20000
#define BASE_PWM 1313
#define PWM_0 7000
#define PWM_12 12700
#else
// 31KHz, TOP 268
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
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
  Serial.println("note off");
  note = 0;
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

  Serial.println("setup complete");
}

void loop() {
  int outputValue = getOutputValue();
  OCR1B = outputValue;
  MIDI.read();
}

int noteToPWM(int note) {
  float scalar = (float)(PWM_12-PWM_0);
  return BASE_PWM+(int)(scalar*pow(2, (float)note/12.0));
}

int getOutputValue() {
  if (note >= 69 && note < 69+19) {
    return noteToPWM(note - 69);
  }
}

