int inPin = D1;

void setup() {
  // put your setup code here, to run once:
  pinMode(inPin,INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  int state = digitalRead(inPin);
  Serial.println(state);
  digitalWrite(LED_BUILTIN,state);
}
