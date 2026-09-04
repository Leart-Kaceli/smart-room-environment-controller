// Pin Assignments
const int tempSensorPin = A0;
const int lightSensorPin = A1;
const int LEDPin = 8;
const int motorPin = 6;

// Thresholds
const int lightThreshold = 260;
const float fanThresholdF = 65.0;

// Global Sensor Variables
int lightSensorValue;
float tempF;
bool roomIsDark;
bool roomIsHot;

void setup() {
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  readSensors();
  controlLighting();
  controlFan();
  printStatus();

  delay(500);
}

//Sensor Reading
void readSensors(){
  lightSensorValue = analogRead(lightSensorPin);

  // Discard the first reading to let the analog pin settle
  // before taking real temp reading
  analogRead(tempSensorPin);
  delay(20);
  int tempSensorValue = analogRead(tempSensorPin);

  float voltage = (tempSensorValue / 1024.0) * 5.0;
  float rawTempC = (voltage - 0.5) * 100;
  float rawTempF = (rawTempC * 9.0 / 5.0) + 32.0;

  // Calibration offset: sensor reads about 10°F high compared
  // to a reference thermometer, so we correct for it here.
  tempF = rawTempF - 10.0;
}

// Lighting Control
void controlLighting(){
  roomIsDark = (lightSensorValue < lightThreshold);
  digitalWrite(LEDPin, roomIsDark ? HIGH : LOW);
}

// Fan Control
void controlFan(){
  // Turn the fan on when room temperature exceeds the selected threshold.
  roomIsHot = (tempF > fanThresholdF);
  digitalWrite(motorPin, roomIsHot ? HIGH : LOW);
}

// Status Reporting
void printStatus(){
  Serial.print("Temperature in Fahrenheit: ");
  Serial.println(tempF);

  Serial.print("Light Level: ");
  Serial.println(lightSensorValue);

  Serial.println(roomIsDark ? "Room Light: ON" : "Room Light: OFF");
  Serial.println(roomIsHot ? "Fan: ON" : "Fan: OFF");
  Serial.println();
}