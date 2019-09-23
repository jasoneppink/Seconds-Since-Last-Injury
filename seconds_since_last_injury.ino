// based on https://playground.arduino.cc/Main/CountUpDownTimer/

#include <IRremote.h>
int recvPin = 53; //IR Receiver (other pins go to 3.3V and GND)
IRrecv irrecv(recvPin);

//Ones SER IN (Serial Data Input) of TPIC6C595
int SerIn_ones = 35;
//Ones SRCK (Shift Register Clock) of TPIC6C595
int SRCK_ones = 33;
//Ones RCK (Register Clock aka latch key) of TPIC6C595
int RCK_ones = 31;

//Tens Digit | Arduino -> TPIC6C595
int SerIn_tens = 32;
int SRCK_tens = 30;
int RCK_tens = 28;

//Hundreds Digit | Arduino -> TPIC6C595
int SerIn_hundreds = 38;
int SRCK_hundreds = 36;
int RCK_hundreds = 34;

//Thousands Digit | Arduino -> TPIC6C595
int SerIn_thousands = 44;
int SRCK_thousands = 42;
int RCK_thousands = 40;


unsigned long Watch, _micro, time = micros();
unsigned int Clock = 0, R_clock;
boolean Reset = false;
volatile boolean timeFlag = false;

void setup()
{

  irrecv.enableIRIn();  // Start the IR receiver
  
  //7 segment LED
  pinMode(SerIn_ones, OUTPUT);
  pinMode(SRCK_ones, OUTPUT);
  pinMode(RCK_ones, OUTPUT);
  pinMode(SerIn_tens, OUTPUT);
  pinMode(SRCK_tens, OUTPUT);
  pinMode(RCK_tens, OUTPUT);
  pinMode(SerIn_hundreds, OUTPUT);
  pinMode(SRCK_hundreds, OUTPUT);
  pinMode(RCK_hundreds, OUTPUT);
  pinMode(SerIn_thousands, OUTPUT);
  pinMode(SRCK_thousands, OUTPUT);
  pinMode(RCK_thousands, OUTPUT);

  DebugOnStartup();

  digitalWrite(RCK_ones, LOW);
  shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B00000000);
  digitalWrite(RCK_ones, HIGH);
  Serial.println("ones off");

  digitalWrite(RCK_tens, LOW);
  shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B00000000);
  digitalWrite(RCK_tens, HIGH);
  Serial.println("tens off");

  digitalWrite(RCK_hundreds, LOW);
  shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B00000000);
  digitalWrite(RCK_hundreds, HIGH);
  Serial.println("hundreds off");

  digitalWrite(RCK_thousands, LOW);
  shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B00000000);
  digitalWrite(RCK_thousands, HIGH);
  Serial.println("thousands off");
  
  Serial.begin(9600);
  StartTimer();
}

void loop()
{ 
  decode_results  results;        // Somewhere to store the IR input results
  
  CountUpTimer(); // run the timer
  digitalWrite(48, HIGH); //turn button LED on

  if (irrecv.decode(&results)) {  // Grab an IR code
    if (buttonPressed(&results)) {

      Serial.println("button pressed");
      Clock = 0;
   
      //immediately turn all off
       digitalWrite(RCK_ones, LOW);
       shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B00000000);
       digitalWrite(RCK_ones, HIGH);
       digitalWrite(RCK_tens, LOW);
       shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B00000000);
       digitalWrite(RCK_tens, HIGH);
       digitalWrite(RCK_hundreds, LOW);
       shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B00000000);
       digitalWrite(RCK_hundreds, HIGH);
       digitalWrite(RCK_thousands, LOW);
       shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B00000000);
       digitalWrite(RCK_thousands, HIGH);
    }
    irrecv.resume();              // Prepare for the next value
  }
  

  // this prevents the time from being constantly shown.
  if (TimeHasChanged() ) 
  {
    if (Clock > 999) {
      Serial.print(DisplayThousands());
      digitalWrite(RCK_thousands, LOW);
      shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, ConvertToByte(DisplayThousands()));
      digitalWrite(RCK_thousands, HIGH);
    } else {
      digitalWrite(RCK_thousands, LOW);
      shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B00000000);
      digitalWrite(RCK_thousands, HIGH);
    }

    if (Clock > 99) {
      Serial.print(DisplayHundreds());
      digitalWrite(RCK_hundreds, LOW);
      shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, ConvertToByte(DisplayHundreds()));
      digitalWrite(RCK_hundreds, HIGH);
    } else {
      digitalWrite(RCK_hundreds, LOW);
      shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B00000000);
      digitalWrite(RCK_hundreds, HIGH);
    }

    if (Clock > 9) {
      Serial.print(DisplayTens());
      digitalWrite(RCK_tens, LOW);
      shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, ConvertToByte(DisplayTens()));
      digitalWrite(RCK_tens, HIGH);
    } else {
      digitalWrite(RCK_tens, LOW);
      shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B00000000);
      digitalWrite(RCK_tens, HIGH);
    }

    if (Clock > 0) {
      Serial.println(DisplayOnes());
      digitalWrite(RCK_ones, LOW);
      shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, ConvertToByte(DisplayOnes()));
      digitalWrite(RCK_ones, HIGH);
    }
    
    if (Clock == 0) {
       //turn ones digit to "0" and all other digits off (otherwise they'll be stuck at 9)
       digitalWrite(RCK_ones, LOW);
       shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B11111010);
       digitalWrite(RCK_ones, HIGH);
       digitalWrite(RCK_tens, LOW);
       shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B00000000);
       digitalWrite(RCK_tens, HIGH);
       digitalWrite(RCK_hundreds, LOW);
       shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B00000000);
       digitalWrite(RCK_hundreds, HIGH);
       digitalWrite(RCK_thousands, LOW);
       shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B00000000);
       digitalWrite(RCK_thousands, HIGH);
    }
  }
}

boolean CountUpTimer()
{
  static unsigned long duration = 1000000; // 1 second
  timeFlag = false;

  // check the time difference and see if 1 second has elapsed
  if ((_micro = micros()) - time > duration ) 
  {
    //Roll over at 9999 because we only have 4 digits right now...
    if(Clock==9999) {
       Clock = 0;
    } else {
       Clock++;
    }
    
    timeFlag = true;

   // check to see if micros() has rolled over, if not,
   // then increment "time" by duration
    _micro < time ? time = _micro : time += duration; 
  }
}

void ResetTimer()
{
    Clock = 0;
}

void StartTimer()
{
  Watch = micros(); // get the initial microseconds at the start of the timer
  Clock = 0;
}

int DisplayThousands()
{
  if ((Clock > 999 && (Clock / 1000U) % 10 > 0)) {
    return (Clock / 1000U) % 10;
  } else {
    return 0;
  }
}

int DisplayHundreds()
{
  if ((Clock > 99) && (Clock / 100U) % 10 > 0) {
    return (Clock / 100U) % 10;
  } else {
    return 0;
  }
}

int DisplayTens()
{
  if ((Clock > 9 && (Clock / 10U) % 10 > 0)) {
    return (Clock / 10U) % 10;
  } else {
    return 0;
  }
}

int DisplayOnes()
{
  return (Clock / 1U) % 10; 
}

byte ConvertToByte(int digit)
{
  switch(digit){
    case 0:
      return B11111010;
    case 1:
      return B11000000;
    case 2:
      return B01101110;
    case 3:
      return B11100110;
    case 4:
      return B11010100;
    case 5:
      return B10110110;
    case 6:
      return B10111110;
    case 7:
      return B11100000;
    case 8:
      return B11111110;
    case 9:
      return B11110100;
  }
}

boolean TimeHasChanged()
{
  return timeFlag;
}

void DebugOnStartup()
{
//Display all numbers on startup
//1
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B11000000);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B11000000);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B11000000);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B11000000);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //2
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B01101110);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B01101110);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B01101110);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B01101110);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //3
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B11100110);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B11100110);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B11100110);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B11100110);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //4
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B11010100);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B11010100);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B11010100);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B11010100);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //5
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B10110110);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B10110110);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B10110110);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B10110110);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //6
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B10111110);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B10111110);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B10111110);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B10111110);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //7
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B11100000);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B11100000);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B11100000);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B11100000);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //8
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B11111110);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B11111110);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B11111110);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B11111110);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //9
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B11110100);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B11110100);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B11110100);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B11110100);
    digitalWrite(RCK_thousands, HIGH);
    delay(500);

    //0
    digitalWrite(RCK_ones, LOW);
    shiftOut(SerIn_ones, SRCK_ones, LSBFIRST, B11111010);
    digitalWrite(RCK_ones, HIGH);
    digitalWrite(RCK_tens, LOW);
    shiftOut(SerIn_tens, SRCK_tens, LSBFIRST, B11111010);
    digitalWrite(RCK_tens, HIGH);
    digitalWrite(RCK_hundreds, LOW);
    shiftOut(SerIn_hundreds, SRCK_hundreds, LSBFIRST, B11111010);
    digitalWrite(RCK_hundreds, HIGH);
    digitalWrite(RCK_thousands, LOW);
    shiftOut(SerIn_thousands, SRCK_thousands, LSBFIRST, B11111010);
    digitalWrite(RCK_thousands, HIGH);
    delay(500); 
}


bool  buttonPressed (decode_results *results)
{
  if((results->decode_type == SONY) && (results->value == 2704)) {
    //2704 = A90
    return true;
  } else {
    return false;
  }
}
