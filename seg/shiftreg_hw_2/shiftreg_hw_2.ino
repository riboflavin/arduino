//**************************************************************//
//  Name    : shiftOutCode, Predefined Dual Array Style         //
//  Author  : Carlyn Maw, Tom Igoe                              //
//  Date    : 25 Oct, 2006                                      //
//  Version : 1.0                                               //
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                            //
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

//holders for infromation you're going to pass to shifting function
byte dataRED;
byte dataGREEN;
byte dataArrayRED[10];
byte dataArrayGREEN[10];

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  Serial.begin(9600);

  //Arduino doesn't seem to have a way to write binary straight into the code 
  //so these values are in HEX.  Decimal would have been fine, too. 
  dataArrayRED[0] = 0xFF; //11111111
  dataArrayRED[1] = 0xFE; //11111110
  dataArrayRED[2] = 0xFC; //11111100
  dataArrayRED[3] = 0xF8; //11111000
  dataArrayRED[4] = 0xF0; //11110000
  dataArrayRED[5] = 0xE0; //11100000
  dataArrayRED[6] = 0xC0; //11000000
  dataArrayRED[7] = 0x80; //10000000
  dataArrayRED[8] = 0x00; //00000000
  dataArrayRED[9] = 0xE0; //11100000

  //Arduino doesn't seem to have a way to write binary straight into the code 
  //so these values are in HEX.  Decimal would have been fine, too. 
  dataArrayGREEN[0] = 0xFF; //11111111
  dataArrayGREEN[1] = 0x7F; //01111111
  dataArrayGREEN[2] = 0x3F; //00111111
  dataArrayGREEN[3] = 0x1F; //00011111
  dataArrayGREEN[4] = 0x0F; //00001111
  dataArrayGREEN[5] = 0x07; //00000111
  dataArrayGREEN[6] = 0x03; //00000011
  dataArrayGREEN[7] = 0x01; //00000001
  dataArrayGREEN[8] = 0x00; //00000000
  dataArrayGREEN[9] = 0x07; //00000111

  //function that blinks all the LEDs
  //gets passed the number of blinks and the pause time
  blinkAll_2Bytes(2,500); 
}

void loop() {

  for (int j = 0; j < 16; j++) {
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);
    //load the light sequence you want from array
    if (j <= 7) {
    dataRED = dataArrayRED[j];
    //move 'em out
    shiftOut(dataPin, clockPin, dataRED);
    }
    if (j > 7) {
    dataGREEN = dataArrayGREEN[j];
    //move 'em out
    shiftOut(dataPin, clockPin, dataGREEN);   
    }
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    delay(300);
  }
}



// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {	
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}


//blinks the whole register based on the number of times you want to 
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}