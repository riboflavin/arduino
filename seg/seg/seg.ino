// Arduino 7 segment display example software
// http://www.hacktronics.com/Tutorials/arduino-and-7-segment-led.html
// License: http://www.opensource.org/licenses/mit-license.php (Go crazy)
 
// Define the LED digit patters, from 0 - 9
// Note that these patterns are for common cathode displays
// For common anode displays, change the 1's to 0's and 0's to 1's
// 1 = LED on, 0 = LED off, in this order:
//                                    Arduino pin: 2,3,4,5,6,7,8
byte seven_seg_digits[10][8] = 
{ 
//{ 1,1,1,1,1,1,0 },  // = 0
//{ 0,1,1,0,0,0,0 },  // = 1
//{ 1,1,0,1,1,0,1 },  // = 2
//{ 1,1,1,1,0,0,1 },  // = 3
//{ 0,1,1,0,0,1,1 },  // = 4
//{ 1,0,1,1,0,1,1 },  // = 5
//{ 1,0,1,1,1,1,1 },  // = 6
//{ 1,1,1,0,0,0,0 },  // = 7
//{ 1,1,1,1,1,1,1 },  // = 8
//{ 1,1,1,0,0,1,1 }   // = 9
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

void setup() {                
  pinMode(2, OUTPUT);   
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  writeDot(1);  // start with the "dot" off
}

void writeDot(byte dot) {
  digitalWrite(9, dot);
}
    
void sevenSegWrite(byte digit) {
  byte pin = 2;
  byte segInv = 0;
  for (byte segCount = 0; segCount < 7; ++segCount) {
    segInv = seven_seg_digits[digit][segCount];
    //this is common anode, so convert 1s to 0s and vice versa
    segInv = (-1 * segInv) + 1;
    digitalWrite(pin, segInv);
    ++pin;
  }
}

void loop() {
  for (byte count = 10; count > 0; --count) {
   delay(1000);
   sevenSegWrite(count - 1); 
  }
  //delay(1000);
}

