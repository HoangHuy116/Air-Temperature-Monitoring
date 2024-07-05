#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2      // Set up the pin to receive the DHT sensor signal
#define DHTTYPE DHT11 // Define the type of sensor
#define RELAY_PIN 7   // Set up the pin to receive the relay signal
#define ENCODER_PIN 3 // Set up the pin to receive the encoder signal

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS; // Assign a variable to determine the library

int count = 0; // Count 
int rpm = 0;   // Rotational speed
void count_pulse() {
  count++; 
}

int setTemperature = 50; // Default temperature to turn on the relay

int waitTime = 1000; 
unsigned long currentTime;
unsigned long lastTime;

void setup() {
  Serial.begin(9600); // Assign digital pins to connect the devices
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(ENCODER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), count_pulse, RISING);
}

void loop() {
  // Read the value from the DHT sensor
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float temperature = event.temperature;
  // Read data from the serial to turn the relay on/off
  if (Serial.available() > 0) {
    setTemperature = Serial.parseInt(); 
  }
  // Send the temperature data via serial in JSON format
  Serial.print("{\"temperature\":");
  Serial.print(temperature);
  Serial.print(", ");
  // Control the relay based on the measured value
  if (temperature > setTemperature) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }
  // Encoder code
  currentTime = millis();
  if (currentTime - lastTime >= waitTime) {
    lastTime = currentTime;
    rpm = (count / 20) * 60;
    // Send the encoder data via serial in JSON format
    Serial.print("\"rpm\":");
    Serial.print(rpm);
    Serial.println("}");
    count = 0;
  }

  delay(2000);
}
