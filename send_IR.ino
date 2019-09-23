// based on IRsendDemo by Ken Shirriff - http://arcfn.com

#include <IRremote.h>

IRsend irsend;
int button_val = 0;

void setup()
{
  //button
  pinMode(6, OUTPUT); //button LED
  pinMode(9, INPUT_PULLUP); //resets counter to zero
}



void loop() {

  digitalWrite(6, HIGH); //turn button LED on

  button_val = digitalRead(9);  // read input value
  if (button_val == HIGH) {      // check if the input is HIGH (button released)
    digitalWrite(6, HIGH);
  } else {
    Serial.println("button pressed");
    digitalWrite(6, LOW);
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa90, 13); // This can be anything. Just make sure to change expected signal on receiving end
      delay(40);
    }
  }
}
