int relay = 7;

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  Serial.println("Enter how many ml you want to pump:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    float ml = input.toFloat();
    if (ml > 0) {
      pumpMl(ml);
      Serial.println("Pumping complete.");
    } else {
      Serial.println("Invalid input. Please enter a number.");
    }
  }
}

void pumpMl(float ml) {
  float seconds = ml / 1.1111111; // flowrate (ml/s)
  int errorRate = (ml < 20) ? 1000 : 3000;
  unsigned long milliseconds = (seconds * 1000) - errorRate;

  Serial.print("Pumping for ");
  Serial.print(seconds);
  Serial.println(" secs.");

  if (milliseconds > 0) {
  digitalWrite(relay, HIGH);
  delay(milliseconds);
  digitalWrite(relay, LOW);
} else {
  Serial.print("No way");
}
}
