/**
 * A tiny synth
 * based on code by Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-stream/streams-generator-i2s/README.md 
 * @copyright GPLv3
 */

#include "AudioTools.h"
#include "Midi.h"
#include "StkAll.h"

MidiCallbackAction action;
MidiBleServer ble("TinySynth", &action);

uint16_t sample_rate=44100;
uint8_t channels = 1;
Voicer voicer;
Clarinet voice1;
Clarinet voice2;
Clarinet voice3;
Clarinet voice4;
I2SStream i2s;
ArdStreamOut out(i2s, channels);
MusicalNotes notes;

void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    StkFloat noteNumber = static_cast<StkFloat>(note);
    StkFloat amplitude = static_cast<StkFloat>(velocity);
    voicer.noteOn(noteNumber, amplitude);
}

void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
  StkFloat noteNumber = static_cast<StkFloat>(note);
  StkFloat amplitude = 0.5; // My MIDI controller doesn't send Note Off velocity.
  voicer.noteOff(noteNumber, amplitude);
}

void setup(void) {  
  // Open Serial 
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  action.setCallbacks(onNoteOn, onNoteOff);
  ble.begin(action);

  // start I2S
  auto config = i2s.defaultConfig(TX_MODE);
  config.pin_ws = 25;
  config.pin_data = 33;
  config.pin_bck = 32;
  config.i2s_format = I2S_STD_FORMAT;
  config.sample_rate = sample_rate; 
  config.channels = channels;
  config.bits_per_sample = 16;
  i2s.begin(config);

  voicer.addInstrument(&voice1);
  voicer.addInstrument(&voice2);
  voicer.addInstrument(&voice3);
  voicer.addInstrument(&voice4);
}

void loop() {
  out.tick( voicer.tick() );
}
