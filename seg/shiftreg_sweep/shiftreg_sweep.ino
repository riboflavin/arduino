//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

int seq1[14] = {1,2,4,8,16,32,64,128,64,32,16,8,4,2};  //The array for storing the 
						// byte #1 value
int seq2[14] = {128,64,32,16,8,4,2,1,2,4,8,16,32,64};  //The array for storing the 
						// byte #2 value

void setup()
{
    pinMode(dataPin, OUTPUT);       //Configure each IO Pin
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

void loop()
{
    for (int x = 0; x < 14; x++)         //Array Index
    {
        digitalWrite(latchPin, LOW);            //Pull latch LOW to start sending data
        shiftOut(dataPin, clockPin, MSBFIRST, seq1[x]);         //Send the data byte 1
        shiftOut(dataPin, clockPin, MSBFIRST, seq2[x]);         //Send the data byte 2
        digitalWrite(latchPin, HIGH);           //Pull latch HIGH to stop sending data
        delay(75);
    }
}
