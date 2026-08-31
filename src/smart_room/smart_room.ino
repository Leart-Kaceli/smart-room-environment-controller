const int tempSensorPin = A0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  int tempSensorValue = analogRead(tempSensorPin);

  float voltage = (tempSensorValue/1024.0) * 5.0;
  float tempC = (voltage - .5) * 100;
  Serial.println("Temperature in Celcius: ");
  Serial.println(tempC);
  float tempF = (tempC * 9.0/5.0) + 32.0;
  Serial.println("Temperature in Farenheit ");
  Serial.println(tempF);

  delay(500);
}