int relay1 = 7;
int relay2 = 8;

void setup() {
  Serial.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  Serial.println("Enter pump number (1 or 2) followed by ml (e.g. 1 50):");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    int spaceIndex = input.indexOf(' ');
    if (spaceIndex > 0) {
      int pumpNumber = input.substring(0, spaceIndex).toInt();
      float ml = input.substring(spaceIndex + 1).toFloat();

      if ((pumpNumber == 1 || pumpNumber == 2) && ml > 0) {
        pumpMl(pumpNumber, ml);
        Serial.println("Pumping complete.");
      } else {
        Serial.println("Invalid input. Format: <pump_number> <ml>");
      }
    } else {
      Serial.println("Invalid format. Please use: <pump_number> <ml>");
    }
  }
}

void pumpMl(int pumpNumber, float ml) {
  float seconds = ml / 1.1111111; // flowrate (ml/s)
  int errorRate = (ml < 20) ? 1000 : 3000;
  unsigned long milliseconds = (seconds * 1000) - errorRate;

  Serial.print("Pumping for ");
  Serial.print(seconds);
  Serial.println(" secs.");

  if (milliseconds > 0) {
    int relayPin = (pumpNumber == 1) ? relay1 : relay2;
    digitalWrite(relayPin, HIGH);
    delay(milliseconds);
    digitalWrite(relayPin, LOW);
  } else {
    Serial.println("Invalid pump time calculated.");
  }
}
