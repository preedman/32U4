#include <Pololu3piPlus32U4.h>

using namespace Pololu3piPlus32U4;

// based on this https://youtu.be/96FuKrPOUOo

// Travel a distance of 30 cm

Encoders encoders;
Buzzer buzzer;
Motors motors;
ButtonA buttonA;
LCD lcd;

unsigned long currentMillis;  // current time
unsigned long prevMillis;     // previous time
const unsigned long PERIOD = 20;  // amount of time to wait between checking - in milliseconds

long countsLeft = 0;
long countsRight = 0;
long prevLeft = 0;
long prevRight = 0;

//The encoders provide a resolution of 12 counts per revolution of the motor shaft when counting both edges of both channels. 
//To compute the counts per revolution of the wheels, multiply the gearboxes’ gear ratios by 12. 
//For example, if 30:1 motors (which have gear ratios more accurately specified as 29.86:1) are used, the encoders provide 29.86 × 12 ≈ 358.3 CPR.
// https://www.pololu.com/docs/0J83/5.4

// Total number of clicks per wheel rotation = CLICKS_PER_ROTATION * GEAR_RATIO

const int CLICKS_PER_ROTATION = 12;  // refers to the DC motor and its rotation. There are 12 state changes in the DC motor that are recognised by the encoders.
const float GEAR_RATIO = 29.86F;
const float WHEEL_DIAMETER = 3.2;  // wheel diamter in cm. 32 mm.
const int WHEEL_CIRCUMFERENCE = 10.0531;  // WHEEL_DIAMETER * pi

float Sl = 0.0F;  // distance in cm how far the left wheel has travelled
float Sr = 0.0F;  // distance in cm how far the right wheel has travelled

void setup() {
  // put your setup code here, to run once:

  Serial.begin(57600);
  delay(1000);
  buzzer.play("c32");

}

void loop() {
  // put your main code here, to run repeatedly:
  

  checkEncoders();

}

void checkEncoders() {

  currentMillis = millis();  // current time in milliseconds

  if(currentMillis > prevMillis + PERIOD) {
    countsLeft = countsLeft + encoders.getCountsAndResetLeft();
    countsRight = countsRight + encoders.getCountsAndResetRight();

    Sl += ((countsLeft - prevLeft) / (CLICKS_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE);  // convert distance in cm
    Sr += ((countsRight - prevRight) / (CLICKS_PER_ROTATION * GEAR_RATIO) * WHEEL_CIRCUMFERENCE); // convert distance in cm

    int wheelSpeed = 75;
    if (Sl < 30.5) {   // if the distance is less than 30.5 cm
      if(Sl > 20) {    // if the distance is greater than 20 cm
        wheelSpeed = 75 * ((30 - Sl) / 10);   // start slowing down
        if(wheelSpeed < 20) wheelSpeed = 20;   // if very close to 30 cm, then slow down even further
      }
       motors.setSpeeds(wheelSpeed, wheelSpeed);
    }
    else {
       motors.setSpeeds(0,0);  // stop
       lcd.gotoXY(0, 0);
       lcd.print(Sl);   // print distance of left wheel, in cm
    }
    
    prevLeft = countsLeft;
    prevRight = countsRight;
    prevMillis = currentMillis;    // remember the previous time
  }
  
}
