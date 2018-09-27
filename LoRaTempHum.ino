#include <TheThingsNetwork.h>
#include <DHT.h>
#include <DHT_U.h>

const char *appEui = "****************";
const char *appKey = "********************************";

// Connect the DHT Shield Power pin to 3.3V
// Connect the DHT Shield Ground pin to ground
// Connect the DHT Shield D4 pin to digital pin 2

#define loraSerial Serial1
#define debugSerial Serial
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);
DHT dht(2, DHT22);

void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  dht.begin();
}

void loop() {
  debugSerial.println("-- LOOP");

  uint32_t humidity = dht.readHumidity(false) * 100;
  uint32_t temperature = dht.readTemperature(false) * 100;

  //  debugSerial.println("Humidity: " + String(humidity));
  //  debugSerial.println("Temperature: " + String(temperature));

  byte payload[4];
  payload[0] = highByte(humidity);
  payload[1] = lowByte(humidity);
  payload[2] = highByte(temperature);
  payload[3] = lowByte(temperature);

  ttn.sendBytes(payload, sizeof(payload));

  delay(10000);
}
