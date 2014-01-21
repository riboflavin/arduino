// Arduino 7 segment display example software
// http://www.hacktronics.com/Tutorials/arduino-and-7-segment-led.html
// License: http://www.opensource.org/licenses/mit-license.php (Go crazy)
 
// Define the LED digit patters, from 0 - 9
// Note that these patterns are for common cathode displays
// For common anode displays, change the 1's to 0's and 0's to 1's
// 1 = LED on, 0 = LED off, in this order:
//                                    Arduino pin: 2,3,4,5,6,7,8

//modiifed to work with two 7seg leds and two shift registers

byte seven_seg_digits[10][8] = 
{ 
//I think you have to have an entire byte here to work with the shift registers
//{ 0,0,0,0,0,0,0,0 },  // = test
{ 1,1,1,1,1,1,0,0 },  // = 0
{ 0,1,1,0,0,0,0,0 },  // = 1
{ 1,1,0,1,1,0,1,0 },  // = 2
{ 1,1,1,1,0,0,1,0 },  // = 3
{ 0,1,1,0,0,1,1,0 },  // = 4
{ 1,0,1,1,0,1,1,0 },  // = 5
{ 1,0,1,1,1,1,1,0 },  // = 6
{ 1,1,1,0,0,0,0,0 },  // = 7
{ 1,1,1,1,1,1,1,0 },  // = 8
{ 1,1,1,0,0,1,1,0 }   // = 9
};

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

void setup() {
  pinMode(dataPin, OUTPUT);       //Configure each IO Pin
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  //Serial.begin(9600);
}

void sevenSegWrite(int count) {
  //split count int into two bytes (only works with two-digit ints for now)
  int count_l = count%10;
  int count_h = (count - count%10)/10;

  //Serial.println("Count is ");
  //Serial.println(count);
  //Serial.println(count_h);
  //Serial.println(count_l);
  
  //set up vars to store the bytes corresponding to the 7seg leds that 
  //will need to light up
  byte count_seg_h[8];
  byte count_seg_l[8];
  
  //make the byte will we send out to the shift register equal
  //to the byte array defining the leds we need
  //this 7seg is common anode, so use ~ to convert 1s to 0s and vice versa
  
  for (int i = 0; i < 8; i++)
  {
   count_seg_h[i] = (seven_seg_digits[count_h][i] * -1) + 1; 
   count_seg_l[i] = (seven_seg_digits[count_l][i] * -1) + 1; 
  //Serial.println("Bytes are ");
  //Serial.println(count_seg_h[i]);
  //Serial.println(count_seg_l[i]);
  }
  
  //next, we need to convert the bit arrays e.g. {1,1,1,1,0,0,0,0} to integers
  //to do this, loop through the array. for each item, multiply it by 2^i, for example when
  //i = 3, {1 or 0} will be multiplied by 2^3 giving 8 or 0

  int i_count_l = 0;
  int i_count_h = 0;

  for (int i = 0; i < 8; i++)
  {
   bitWrite(i_count_h,i,count_seg_h[i]); 
   bitWrite(i_count_l,i,count_seg_l[i]); 
  //Serial.println("Bytes are ");
  //Serial.println(i_count_h);
  //Serial.println(i_count_l);
  }

  digitalWrite(latchPin, LOW); //Pull latch LOW to start sending data
  shiftOut(dataPin, clockPin, MSBFIRST, i_count_l); //Send the data byte 1
  shiftOut(dataPin, clockPin, MSBFIRST, i_count_h); //Send the data byte 2
  digitalWrite(latchPin, HIGH); //Pull latch HIGH to stop sending data    
  digitalWrite(latchPin, LOW); //Pull latch LOW to start sending data
}

void loop() {
  for (byte count = 0; count < 100; count++) {
  delay(50);
  sevenSegWrite(count); 
  }
}

