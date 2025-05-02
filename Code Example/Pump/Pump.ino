/*

A simple sketch to test TA7291P

P1  = Arduino GND AND 12V GND
P2  = OUT1 = Positive pump terminal 
P3  = NC
P4  = 12V+
P5  = Arduino D5 (PWM) 
P6  = Arduino D6 (PWM)
P7  = Arduino 5V+
P8  = 12V+
P9  = NC
P10 = OUT2 = Negative pump terminal

This sketch uses PWM on pin D5 and D6 to slowly turn on the pump, then keeps it running at full speed before slowing down to a full stop and restart in reverse.
It starts with a pause of 5 seconds to give you an opportunity to plug/unplug power without creating back EMF.

Open the Serial Monitor to watch the PWM value sent to the pump.

This code was created by Francois Marquis (2014) to be part of an instructable (Control peristaltic pump with TA7291P and an Arduino). 
It is PUBLIC DOMAIN and you can freely use it. If you improve it, please send me a new version!

*/

// Variable declaration

int IN1Pin = 5;  // Pin IN1 connected on D5 (PWM), Modify if you use a different PWM pin!
int IN2Pin = 6;  // Pin IN2 connected on D6 (PWM), Modify if you use a different PWM pin!
int ledPin = 13; // Only for debug - Remove as needed
int val = 0;     // Always start with initialized variables! val will be the intensity of the PWM, from 0 (full stop) to 255 (full speed)
boolean pumpMode = false; // false = Forward, true = Reverse

void setup()
{
  Serial.begin(9600);
  pinMode(IN1Pin, OUTPUT);
  pinMode(IN2Pin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  analogWrite(IN1Pin, 0);    // According to the TA7291P datasheet, it is very important to start the bridge at full stop. I will not argue with them!
  analogWrite(IN2Pin, 0);    // Always according to the same datasheet, both pins should be low.
  analogWrite(ledPin, val);  // LED is off when you start
  Serial.println("Waiting, 5s");    // Just to remember you why nothing is working at this point!
  delay(5000);                      // Initial pause, change this value according to your needs (in milliseconds)
}

void loop()
{
 
// This first section starts the pump in forward pumping
if (pumpMode == false)
 {
  if (val < 255 )
  {
    analogWrite(IN1Pin, val);
    analogWrite(ledPin, val);
    delay(5);                   // You can tweak this value, This is the actual waiting time before increasing to the next PWM value
    Serial.println(val);        // Will print the value of val (current PWM) to the serial monitor
    val++;                      // The cycle repeats itself until max speed is reached.
  }
  else
  {
    analogWrite(IN1Pin, 255);      // Now, you are at max speed
    analogWrite(ledPin, 255);      // The LED is fully illuminated
    Serial.println("Full speed, forward, 10s");
    delay(10000);                  // The pump will stay at max speed for 10 seconds, edit this value to your liking
    for (int x = 0; x <= 254; x++) // This will slow down the pump to a full stop 
    {
      val--;
      analogWrite(IN1Pin, val);
      analogWrite(ledPin, val);
      delay(5);
      Serial.println(val);
    }
    Serial.println("Waiting, 5s and preparing for reverse pumping");    
    pumpMode = !pumpMode;                                               // Set the pump in reverse
    delay(5000);                                                        // And pause the pump for 5 seconds before reversing polarity and restarting backward.
  }
}
else
 {
   if (val < 255)
  {
    analogWrite(IN2Pin, val);     // Same code, but notice how now IN2Pin is used while IN1Pin stays at zero
    analogWrite(ledPin, val);
    delay(5);                 
    Serial.println(val);        
    val++;
  }
  else
  {
    analogWrite(IN2Pin, 255);
    analogWrite(ledPin, 255);
    Serial.println("Full speed, reverse, 10s");
    delay(10000);
    for (int x = 0; x <= 254; x++)
    {
      val--;
      analogWrite(IN2Pin, val);
      analogWrite(ledPin, val);
      delay(5);
      Serial.println(val);
    }
    Serial.println("Waiting, 5s and preparing for forward pumping");
    pumpMode = !pumpMode;
    delay(5000);
  }
 }
}