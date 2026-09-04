#include  <LiquidCrystal.h>


LiquidCrystal lcd(7, 9, 2, 3, 4, 5);

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

// LCD screen-cycling variables
const unsigned long screenInterval = 2000;  // time each screen is shown (ms)
unsigned long lastScreenChange = 0;
int currentScreen = 0;  // 0 = Temp/Light Level, 1 = Fan/Light Status

void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(LEDPin, OUTPUT);
  pinMode(motorPin, OUTPUT);

  lcd.print("Smart Room");
  lcd.setCursor(0,1);
  lcd.print("Controller");
  delay(2000);
  lcd.clear();
}

void loop() {
  readSensors();
  controlLighting();
  controlFan();
  printStatus();
  updateLCD();

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

// Status Reporting (Serial Monitor)
void printStatus(){
  Serial.print("Temperature in Fahrenheit: ");
  Serial.println(tempF);

  Serial.print("Light Level: ");
  Serial.println(lightSensorValue);

  Serial.println(roomIsDark ? "Room Light: ON" : "Room Light: OFF");
  Serial.println(roomIsHot ? "Fan: ON" : "Fan: OFF");
  Serial.println();
}

// Status Reporting (LCD)
void updateLCD(){
// Check if it's time to switch to the next screen
  unsigned long currentTime = millis();
  if (currentTime - lastScreenChange >= screenInterval) {
    currentScreen = (currentScreen + 1) % 2;  // toggle between screen 0 and 1
    lastScreenChange = currentTime;
    lcd.clear(); 
  }

  if (currentScreen == 0) {
    showTempAndLightScreen();
  } else {
    showFanAndLightStatusScreen();
  }
}

// LCD Screen 0: Temperature + Light Level
void showTempAndLightScreen() {
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(tempF, 1);   // 1 decimal place
  lcd.print("F   ");     // trailing spaces clear leftover digits

  lcd.setCursor(0, 1);
  lcd.print("Light Lvl: ");
  lcd.print(lightSensorValue);
  lcd.print("   ");      // trailing spaces clear leftover digits
}

// LCD Screen 1: Fan + Light Status
void showFanAndLightStatusScreen() {
  lcd.setCursor(0, 0);
  lcd.print("Fan:  ");
  lcd.print(roomIsHot ? "ON " : "OFF");

  lcd.setCursor(0, 1);
  lcd.print("Light:");
  lcd.print(roomIsDark ? "ON " : "OFF");
}