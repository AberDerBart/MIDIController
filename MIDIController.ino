#include "MIDIUSB.h"

#define N_STRINGS 6
#define STRING_PINS  1,  2,  3,  4,  5,  6
#define STRING_NOTES 76, 71, 67, 62, 57, 52

#define N_FRETS 5
#define FRET_PINS 7, 8, 9, 10, 11

const uint8_t frets[] = {
	FRET_PINS
};

const uint8_t strings[] = {
	STRING_PINS
};

const uint8_t stringNotes[] = {
	STRING_NOTES
};

struct stringState{
	uint8_t prevFret;
	uint8_t currentFret;
};

struct stringState stringStates[N_STRINGS];

void startNote(const uint8_t note){
	midiEventPacket_t noteOn = {0x09, 0x90 | 0, note, 127};
	MidiUSB.sendMIDI(noteOn);
}

void stopNote(const uint8_t note){
	midiEventPacket_t noteOff = {0x08, 0x80 | 0, note, 0};
	MidiUSB.sendMIDI(noteOff);
}

void setup(){
	for(int i = 0; i < N_FRETS; i++){
		pinMode(frets[i], OUTPUT);
	}

	for(int j = 0; j < N_STRINGS; j++){
		pinMode(strings[j], INPUT);
		stringStates[j] = {0, 0};
	}
}

void loop(){

	for(int i = 0; i < N_FRETS; i++){
		digitalWrite(frets[i], HIGH);	

		for(int j = 0; j < N_STRINGS; j++){
			if(digitalRead(strings[j])){
				stringStates[j].currentFret = i;
			}
		}

		digitalWrite(frets[i], LOW);
	}

	for(int j = 0; j < N_STRINGS; j++){
		if(stringStates[j].prevFret != stringStates[j].currentFret){
			if(stringStates[j].prevFret){
				stopNote(stringNotes[j] + stringStates[j].prevFret);
			}

			if(stringStates[j].currentFret){
				startNote(stringNotes[j] + stringStates[j].currentFret);
			}
		}

		stringStates[j].prevFret = stringStates[j].currentFret;
		stringStates[j].currentFret = 0;
	}

	MidiUSB.flush();
}
