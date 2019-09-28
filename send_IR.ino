// based on IRsendDemo by Ken Shirriff - http://arcfn.com

#include <IRremote.h>

IRsend irsend;
int button_val = 0;

void setup()
{
  //button
  pinMode(10, OUTPUT); //button LED
  pinMode(11, INPUT_PULLUP); //resets counter to zero

  //if Arduino gets rebooted by EMI, reset upon reboot
  for (int i = 0; i < 3; i++) {
    irsend.sendSony(0xa90, 13); // This can be anything. Just make sure to change expected signal on receiving end
    delay(40);
  }
}



void loop() {

  digitalWrite(10, HIGH); //turn button LED on

  button_val = digitalRead(11);  // read input value
  if (button_val == HIGH) {      // check if the input is HIGH (button released)
    digitalWrite(10, HIGH);
  } else {
    Serial.println("button pressed");
    digitalWrite(10, LOW);
    for (int i = 0; i < 3; i++) {
      irsend.sendSony(0xa90, 13); // This can be anything. Just make sure to change expected signal on receiving end
      delay(40);
    }
  }
}
