int relayPins[] = {7, 8, 9, 10}; // Pump C, M, Y, K

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
  }

  Serial.println("Enter:");
  Serial.println("- '<pump_number> <ml>' for manual pumping (for example 1 50 / mnaual mode)");
  Serial.println("- 'hex #RRGGBB' to mix a color");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("hex ")) {
      String hexCode = input.substring(4);
      mixFromHex(hexCode, 30.0); // total volume = 30 ml
    } else {
      int spaceIndex = input.indexOf(' ');
      if (spaceIndex > 0) {
        int pumpNumber = input.substring(0, spaceIndex).toInt();
        float ml = input.substring(spaceIndex + 1).toFloat();

        if ((pumpNumber >= 1 && pumpNumber <= 4) && ml > 0) {
          pumpMl(pumpNumber, ml);
          Serial.println("Pumping complete.");
        } else {
          Serial.println("Invalid input. Format: <pump_number> <ml>");
        }
      } else {
        Serial.println("Invalid input. Try: '1 50' or 'hex #RRGGBB'");
      }
    }
  }
}

void pumpMl(int pumpNumber, float ml) {
  float seconds = ml / 1.1111111; // flowrate (ml/s)
  int errorRate = (ml < 20) ? 1000 : 3000;
  unsigned long milliseconds = (seconds * 1000) - errorRate;

  Serial.print("Pump ");
  Serial.print(pumpNumber);
  Serial.print(" → ");
  Serial.print(ml);
  Serial.print("ml for ");
  Serial.print(seconds);
  Serial.println(" secs.");

  if (milliseconds > 0) {
    digitalWrite(relayPins[pumpNumber - 1], HIGH);
    delay(milliseconds);
    digitalWrite(relayPins[pumpNumber - 1], LOW);
  } else {
    Serial.println("Invalid pump time calculated.");
  }
}

void mixFromHex(String hex, float totalMl) {

  hex.trim();
  //BASE CASE HERE
  if (hex.charAt(0) == '#') {
    hex = hex.substring(1);
  }

  if (hex.length() != 6) { // ดัก
    Serial.println("Invalid hex code. Use format #RRGGBB.");
    return;
  }

  int r = strtol(hex.substring(0, 2).c_str(), NULL, 16);
  int g = strtol(hex.substring(2, 4).c_str(), NULL, 16);
  int b = strtol(hex.substring(4, 6).c_str(), NULL, 16);

  float r_ = r / 255.0;
  float g_ = g / 255.0;
  float b_ = b / 255.0;

  float k = 1 - max(r_, max(g_, b_));
  float c = 0, m = 0, y = 0;

  if (k < 1.0) {
    c = (1 - r_ - k) / (1 - k);
    m = (1 - g_ - k) / (1 - k);
    y = (1 - b_ - k) / (1 - k);
  }

  float cMl = c * totalMl;
  float mMl = m * totalMl;
  float yMl = y * totalMl;
  float kMl = k * totalMl;

  Serial.println("CMYK Breakdown (ml):");
  Serial.print("Cyan: "); Serial.println(cMl);
  Serial.print("Magenta: "); Serial.println(mMl);
  Serial.print("Yellow: "); Serial.println(yMl);
  Serial.print("Black: "); Serial.println(kMl);

  if (cMl > 0) pumpMl(1, cMl);
  if (mMl > 0) pumpMl(2, mMl);
  if (yMl > 0) pumpMl(3, yMl);
  if (kMl > 0) pumpMl(4, kMl);

  Serial.println("ColorDripped completed");
}
