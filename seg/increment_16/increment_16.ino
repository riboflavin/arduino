
//binary counter to 256; works with two shift registers x 8 LEDs each

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

int byte1 = 0;         //The counter for storing the byte #1 value
int byte2 = 0;         //The counter for storing the byte #2 value

void setup()
{
    pinMode(dataPin, OUTPUT);       //Configure each IO Pin
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
}

void loop()
{
    for (byte2 = 0; byte2 < 256; byte2++)               //Outer Loop
    {
        for (byte1 = 0; byte1 < 256; byte1++)            //Inner Loop
        {
            digitalWrite(latchPin, LOW);           //Pull latch LOW to start sending data
            shiftOut(dataPin, clockPin, MSBFIRST, byte1); //Send the data byte 1
            shiftOut(dataPin, clockPin, MSBFIRST, byte2); //Send the data byte 2
            digitalWrite(latchPin, HIGH);          //Pull latch HIGH to stop sending data
            delay(50);
        }
    }
}
