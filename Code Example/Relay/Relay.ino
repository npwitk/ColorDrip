int relay = 8;  

void setup() {
  pinMode(relay, OUTPUT); 
}

void loop() {
  digitalWrite(relay, HIGH); 
  delay(10000);               
  digitalWrite(relay, LOW); 
  delay(5000);           
}