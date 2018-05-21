
// GrayConvertLUT
//
//  Created on: Jan 21, 2018
//      Author: Christopher Ruelle
//
//--Project Header---
//
//
// Created: 10/18/17
// By: Christopher Ruelle
// For: EE408- Senior Design
//
//
//---Project Overview---
//
// This is a function created to receive the 
// gray code from the knob and convert that
// into it's position value on the knob.
//-- Project Details ---
//
// 256 value lookup table
// 8 bit input value of the function
// returns 8 bit value
// The static is needed to keep the LUT from being allocated
// and initialized every time the function is called.
// The const because we don't want to write to this only read.
// The __flash is used to save RAM on the device, however this maybe the
// incorrect call for using flash as I looked up how in general C syntax
// not Arduino
// The gray code has 256 possible values, however it only uses 128 different identifying values.
// This means that there are a great deal of values not used. If there is an error
// due to the gray code being a value it should not be, the LUT will return the value 128 and this will 
// serve as a flag. 
//

#include "switchArrayData.h"
/*
  * CS - to digital pin 8  (SS pin) P2.0
  * SDI - to digital pin 14/15 (MOSI pin USI/USCI) P1.6/P1.7
  * CLK - to digital pin 7 (SCK pin) P1.5
  * GND - to ground
*/

// include the SPI library:
#include <SPI.h>

// set pin 8 as the slave select for the digital pot:
const int slaveSelectPin = SS;
unsigned char grayCodeLookup(unsigned char GRAY_CODE_IN);
unsigned char readKnob();
void digitalSwitchWrite(int value1, int value2, int value3, int value4);

//Knob position ranges
#define mMIC80  89
#define spare1 95
#define spare2 101
#define mline20 107
#define line15 112
#define line10 117
#define line5 123
#define line0 0
#define mline5 6
#define mline10 11
#define OFF1 16
#define mMIC20 22
#define MIC25 26
#define MIC30 33
#define MIC35 38
#define MIC40 43
#define MIC45 48
#define MIC50 53
#define OFF2 58
#define MIC55 64
#define MIC60 69
#define MIC65 74
#define MIC70 79
#define MIC75 84

//Knob connections to uC
#define knob0 P1_0
#define knob1 P2_2
#define knob2 P2_5
#define knob3 P1_1
#define knob4 P1_4
#define knob5 P2_1
#define knob6 P2_3
#define knob7 P2_4



	
int main () {
    unsigned char KNOB_INPUT;
	unsigned char KNOB_POSITION_VALUE = 0; 
	unsigned char SPI_DATA_U1 = 0;
	unsigned char SPI_DATA_U2 = 0;
	unsigned char SPI_DATA_U3 = 0;
	unsigned char SPI_DATA_U4 = 0;
	
    pinMode (slaveSelectPin, OUTPUT);
    // initialize SPI:
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV8);
	//knob input configuration
    pinMode (knob0, INPUT_PULLUP);
    pinMode (knob1, INPUT_PULLUP);
    pinMode (knob2, INPUT_PULLUP);
    pinMode (knob3, INPUT_PULLUP);
    pinMode (knob4, INPUT_PULLUP);
    pinMode (knob5, INPUT_PULLUP);
    pinMode (knob6, INPUT_PULLUP);
    pinMode (knob7, INPUT_PULLUP);
    while (1) {
        KNOB_INPUT = readKnob();
    	KNOB_POSITION_VALUE = grayCodeLookup(KNOB_INPUT);
    	if (KNOB_POSITION_VALUE>=mMIC80 && KNOB_POSITION_VALUE<spare1){ //-80 MIC
	    	SPI_DATA_U1 = (1<<S2A_m80rOFF);
            SPI_DATA_U2 = (1<<S2B_m80rm70);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = (1<<S2C_m80m50);
    	}
    	if (KNOB_POSITION_VALUE>=spare1 && KNOB_POSITION_VALUE<spare2){ //spare 1
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=spare2 && KNOB_POSITION_VALUE<mline20){ //spare 2
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=mline20 && KNOB_POSITION_VALUE<line15){ //-20 Line
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = (1<<S2B_m202);
    	}
    	if (KNOB_POSITION_VALUE>=line15 && KNOB_POSITION_VALUE<line10){ //-15 Line
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = (1<<S2B_m15);
    	}
    	if (KNOB_POSITION_VALUE>=line10 && KNOB_POSITION_VALUE<line5){ //-10 Line
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = (1<<S2B_m10);
    	}
    	if (KNOB_POSITION_VALUE>=line5 && KNOB_POSITION_VALUE<=127){  //-5 Line use of 127 ensures correct behavior here
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = (1<<S2B_m5);
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=line0 && KNOB_POSITION_VALUE<mline5){ //0 Line
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = (1<<S2B_0);
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=mline5 && KNOB_POSITION_VALUE<mline10){ //+5 Line
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = (1<<S2B_5);
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=mline10 && KNOB_POSITION_VALUE<OFF1){ //+10 Line
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = (1<<S2B_10);
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=OFF1 && KNOB_POSITION_VALUE<mMIC20){ //OFF 1
    	    SPI_DATA_U1 = 0;
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = (1<<S2B_OFF2);
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=mMIC20 && KNOB_POSITION_VALUE<MIC25){ //-20 MIC
    	    SPI_DATA_U1 = (1<<S2A_m20);
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = (1<<S2B_m20);
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC25 && KNOB_POSITION_VALUE<MIC30){ //-25 MIC
    	    SPI_DATA_U1 = (1<<S2A_m25);
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = (1<<S2B_m25);
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC30 && KNOB_POSITION_VALUE<MIC35){ //-30 MIC
    	    SPI_DATA_U1 = (1<<S2A_m30);
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = (1<<S2B_m30);
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC35 && KNOB_POSITION_VALUE<MIC40){ //-35 MIC
    	    SPI_DATA_U1 = (1<<S2A_m35);
            SPI_DATA_U2 = (1<<S2B_m35);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC40 && KNOB_POSITION_VALUE<MIC45){ //-40 MIC
    	    SPI_DATA_U1 = (1<<S2A_m40);
            SPI_DATA_U2 = (1<<S2B_m40);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC45 && KNOB_POSITION_VALUE<MIC50){ //-45 MIC
    	    SPI_DATA_U1 = (1<<S2A_m45);
            SPI_DATA_U2 = (1<<S2B_m45);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = (1<<S2C_m75m45);
    	}
    	if (KNOB_POSITION_VALUE>=MIC50 && KNOB_POSITION_VALUE<OFF2){ //-50 MIC
    	    SPI_DATA_U1 = (1<<S2A_m50);
            SPI_DATA_U2 = 0;
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = (1<<S2C_m80m50);
    	}
    	if (KNOB_POSITION_VALUE>=OFF2 && KNOB_POSITION_VALUE<MIC55){ //OFF 2
    	    SPI_DATA_U1 = (1<<S2A_m80rOFF);
            SPI_DATA_U2 = (1<<S2B_OFF);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC55 && KNOB_POSITION_VALUE<MIC60){ //-55 MIC
    	    SPI_DATA_U1 = (1<<S2A_m80rOFF);
            SPI_DATA_U2 = (1<<S2B_m55);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC60 && KNOB_POSITION_VALUE<MIC65){ //-60 MIC
    	    SPI_DATA_U1 = (1<<S2A_m80rOFF);
            SPI_DATA_U2 = (1<<S2B_m60);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC65 && KNOB_POSITION_VALUE<MIC70){  //-65 MIC
    	    SPI_DATA_U1 = (1<<S2A_m80rOFF);
            SPI_DATA_U2 = (1<<S2B_m65);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC70 && KNOB_POSITION_VALUE<MIC75){ //-70 MIC
    	    SPI_DATA_U1 = (1<<S2A_m80rOFF);
            SPI_DATA_U2 = (1<<S2B_m80rm70);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = 0;
    	}
    	if (KNOB_POSITION_VALUE>=MIC75 && KNOB_POSITION_VALUE<mMIC80){ //-75 MIC
			SPI_DATA_U1 = (1<<S2A_m80rOFF);
            SPI_DATA_U2 = (1<<S2B_m80rm70);
        	SPI_DATA_U3 = 0;
	        SPI_DATA_U4 = (1<<S2C_m75m45);
		}
		
		digitalSwitchWrite(SPI_DATA_U1, SPI_DATA_U2, SPI_DATA_U3, SPI_DATA_U4);
	/*	digitalWrite(slaveSelectPin,LOW);
		SPI.transfer(SPI_DATA_U1);
		SPI.transfer(SPI_DATA_U2);
		SPI.transfer(SPI_DATA_U3);
		SPI.transfer(SPI_DATA_U4);
		digitalWrite(slaveSelectPin,HIGH);
	/*	digitalWrite(slaveSelectPin,LOW);
	//	SPI.transfer(0);
	//	SPI.transfer(0);
		SPI.transfer(KNOB_INPUT);
		SPI.transfer(KNOB_POSITION_VALUE);
		digitalWrite(slaveSelectPin,HIGH);*/
		//delay(100);  
    }
}


unsigned char readKnob() {
    unsigned char knobValue = 0;
    knobValue = (digitalRead(knob7) << 7) +
                (digitalRead(knob6) << 6) +
                (digitalRead(knob5) << 5) +
                (digitalRead(knob4) << 4) +
                (digitalRead(knob3) << 3) +
                (digitalRead(knob2) << 2) +
                (digitalRead(knob1) << 1) +
                digitalRead(knob0);
    return knobValue;
}

/*
unsigned char grayCodeLookup(unsigned char KNOB_POSITION_VALUE) {
    const unsigned char GRAY_CODE_LOOKUP_TABLE[256]=
        {128,56,40,55,24,128,39,52,8,57,128,128,
23,128,36,13,120,128,41,54,128,128,128,
53,7,128,128,128,20,19,125,18,104,105,
128,128,25,106,38,128,128,58,128,128,
128,37,14,128,119,118,128,128,128,107,
128,128,4,128,3,128,109,108,2,1,88,128,
89,128,128,128,128,51,9,10,90,128,22,11,
128,12,128,128,42,43,128,128,128,128,128,
128,128,128,21,128,126,127,103,128,102,
128,128,128,128,128,128,128,91,128,128,
128,128,128,116,117,128,128,115,128,128,
128,93,94,92,128,114,95,113,0,72,71,128,
68,73,128,128,29,128,70,128,69,128,128,
35,34,121,128,122,128,74,128,128,30,6,
128,123,128,128,128,124,17,128,128,128,
67,26,128,27,28,128,59,128,128,128,128,
128,15,128,128,128,128,128,128,128,128,
5,128,128,128,110,128,111,16,87,84,128,
45,86,85,128,50,128,128,128,46,128,128,
128,33,128,83,128,44,75,128,128,31,128,
128,128,128,128,128,128,32,100,61,101,
66,128,62,128,49,99,60,128,47,128,128,
128,48,77,82,78,65,76,63,128,64,98,81,
79,80,97,96,112,128};
    unsigned char KNOB_POSITION_VALUE_OUT = 0;

    KNOB_POSITION_VALUE_OUT = GRAY_CODE_LOOKUP_TABLE[KNOB_POSITION_VALUE];
    return KNOB_POSITION_VALUE_OUT;
}
*/
unsigned char grayCodeLookup(unsigned char GRAY_CODE_IN){
	const unsigned char GRAY_CODE_TABLE[128] = {127,63,62,58,56,184,152,24,8,72,73,
	77,79,15,47,175,191,159,31,29,28,92,76,12,4,36,164,166,167,135,151,215,223,
	207,143,142,14,46,38,6,2,18,82,83,211,195,203,235,239,231,199,71,7,23,19,3,
	1,9,41,169,233,225,229,245,247,243,227,163,131,139,137,129,128,132,148,212,
	244,240,242,250,251,249,241,209,193,197,196,192,64,66,74,106,122,120,121,
	125,253,252,248,232,224,226,98,96,32,33,37,53,61,60,188,190,254,126,124,
	116,112,113,49,48,16,144,146,154,158,30,94,95};
	unsigned char index = 0;
	while ((GRAY_CODE_TABLE[index] != GRAY_CODE_IN) && (index < 128)) {
		index++;
	}
	return index;
}
void digitalSwitchWrite(int value1, int value2, int value3, int value4) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin,LOW);
  //  send in the address and value via SPI:
  SPI.transfer(value4);
  SPI.transfer(value3);
  SPI.transfer(value2);
  SPI.transfer(value1);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin,HIGH); 
}