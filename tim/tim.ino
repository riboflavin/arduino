
int ledPins[] = {2,3,4,5,6,7,8,9,10,11};
const int countPins = 10;

//set countPins * 2 array to keep track of pin position for 10 pins and pin state
int ledMap[countPins][2];

//button-related variables
int buttonPin = 13;

//start buttonState at 1, which is off value
int buttonState = 1;
int lastButtonState = 1;

unsigned long lastActionTime = millis();
float timerMillis = 0;

void setup() {
  Serial.begin(9600);
  int i;

  //set initial state for each pin
  for (i = 0; i < countPins; i++)
  {
    ledMap[i][0] = 0;
    ledMap[i][1] = ledPins[i];
    // Serial.println(i);
    // Serial.print("position: ");
    // Serial.println(ledMap[i][1]);
    // Serial.print("state: ");
    // Serial.println(ledMap[i][0]);
  }

  //set pinmode for each pin
  pinMode(buttonPin, INPUT);  
  for (i = 0; i < countPins; i++)
  {
    pinMode(ledMap[i][1], OUTPUT);
  }

}

void refreshDisplay() {
  int i;
  for (i = 0; i < countPins; i++)
  {
    //if led should be on, turn it on
    if (ledMap[i][0] == 1) {digitalWrite(ledMap[i][1],HIGH);}
    //if led should be off, turn it off
    if (ledMap[i][0] == 0) {digitalWrite(ledMap[i][1],LOW);}
  }
}

int barCount() {
  Serial.println("barCount");
  int i;
  int count = 0;
  for (i = 0; i < countPins; i++)
  {
    if (ledMap[i][0] == 1) {count++;}
  }

  return count;
}

void barBlinkOne(int i, int d) {
  Serial.println("barBlinkOne");
  int j;
  if (d < 100) {d = 100;}
  digitalWrite(ledMap[i][1],HIGH);
  delay(d);
  digitalWrite(ledMap[i][1],LOW);
  delay(d);
}

void barBlink() {
  int i;
  int j;

  for (j = 0; j < 4; j++)
  {
    for (i = 0; i < countPins; i++)
    {
      //turn all leds off
      digitalWrite(ledMap[i][1],LOW);
    }

    delay(500);
    refreshDisplay();
    delay(500);
  }
}

void barBlinkAll(int d, int r) {
  int i;
  int j;
  Serial.println("barBlinkAll");

  for (j = 0; j < r; j++)
  {
    for (i = 0; i < countPins; i++)
    {
      //turn all leds off
      digitalWrite(ledMap[i][1],LOW);
    }
    delay(d);

    for (i = 0; i < countPins; i++)
    {
      //turn all leds on
      digitalWrite(ledMap[i][1],HIGH);
    }
    delay(d);
  }

  refreshDisplay();
}

void barAdd()
{
  int i;
  Serial.println("barAdd");
  for (i = 0; i < countPins; i++)
  {
    //iterate until finding an led that is off, then turn it on and end loop
    if (ledMap[i][0] == 0) {
      ledMap[i][0] = 1; 
      i = countPins;
    }
  }

  refreshDisplay();
}
 
void barSubtract()
{
  int i;
  Serial.println("barSubtract");
  Serial.println(countPins);
  for (i = countPins - 1; i >= 0; i--)
  {
    //iterate until finding an led that is on, then turn it off and end loop
    Serial.println("led:");
    Serial.println(i);
    if (ledMap[i][0] == 1) {
      ledMap[i][0] = 0; 
      i = 0;
      Serial.println("turn off");
    }
  }

  refreshDisplay();
}

void loop() {
  refreshDisplay();
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == LOW) {
      // if the current state is LOW then the button
      // went from off to on; add one minute in ms
      timerMillis += (1.0 * 1000.0 * 60.0);
      lastActionTime = millis();
      barAdd();
    } 
    else {
    }
  }
  // save the current state as the last state, 
  // for next time through the loop
  lastButtonState = buttonState;

  // have we been idle for more than 3000ms and set a time? then start countdown
  if (
     ((millis() - lastActionTime) > 3000) &&
     (timerMillis > 0)
     ) 
     {countdown();}

  // have we been idle for more than 3000ms and not set a time? then blink appealingly
  if (
     ((millis() - lastActionTime) > 3000) &&
     (timerMillis == 0)
     ) 
     {
      barBlinkAll(100,2);
      lastActionTime = millis();
     }
}

void countdown() {
  unsigned long lastUpdateTime = millis();

  while (timerMillis > 0) {
    //add extremely slight delay so 0 isn't passed to barBlinkOne the first
    //time this function is run
    delay(1);
    
    //get time elapsed since countdown started and update timing variables
    timerMillis -= (millis() - lastUpdateTime);
    lastUpdateTime = millis();
    Serial.println("timerMillis: ");
    Serial.println(timerMillis);

    while (
          (barCount() - 1) > (timerMillis / 1000.0 / 60.0) &&
          (timerMillis > 0)
          )
          {
            barSubtract();
          }

    //split the minutes remaining into decimal and integer
    //take the decimal, multiply by 1000, and use for blink delay
    double integerPart;
    double fractionPart = modf((timerMillis / 1000.0 / 60.0), &integerPart);
    barBlinkOne(barCount() - 1, fractionPart * 1000);
  }

  barSubtract();
  barBlinkAll(300,3);
  lastActionTime = millis();
  timerMillis = 0;
}