const int tempSensorPin = A0;
const int lightSensorPin = A1;
const int lightThreshold = 260;
const int LEDPin = 8;

void setup() {
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  int lightSensorValue = analogRead(lightSensorPin);
  analogRead(tempSensorPin);
  delay(20);
  int tempSensorValue = analogRead(tempSensorPin);

  float voltage = (tempSensorValue / 1024.0) * 5.0;
  float rawTempC = (voltage - 0.5) * 100;
  float rawTempF = (rawTempC * 9.0 / 5.0) + 32.0;
  float tempF = rawTempF - 10.0;

  Serial.print("Temperature in Fahrenheit: ");
  Serial.println(tempF);

  Serial.print("Light Level: ");
  Serial.println(lightSensorValue);

  bool roomIsDark = (lightSensorValue < lightThreshold);
  digitalWrite(LEDPin, roomIsDark ? HIGH : LOW);
  Serial.println(roomIsDark ? "Room Light: ON" : "Room Light: OFF");

  delay(500);
}