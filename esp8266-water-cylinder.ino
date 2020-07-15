/*
 * esp8266 with DS18B20+ (mains-powered)
 *
 * MQTT Temperature Sensor
 *
 * Designed for integration with HomeAssistant
 *
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.h" // WiFi and device settings

WiFiClient espClient;
PubSubClient client(espClient);

float previousTemperature = 0.0;
float temperature, temperatureDifference;

void setup() {

  // switch on Serial
  Serial.begin(74880);
  while (!Serial) {}
  Serial.setDebugOutput(true);

  Serial.print("Welcome ");
  Serial.println(device);
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  // switch on WiFi
  WiFi.hostname(wifi_hostname);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // configure MQTT server
  client.setServer(mqtt_server, 1883);

} // end setup

void loop() {

  // read latest sensors

  Serial.print("Previous temperature (C): ");
  Serial.println(previousTemperature);
  Serial.print("Current temperature (C): ");
  Serial.println(temperature);

  // calculate difference from last reading
  temperatureDifference = fabs(previousTemperature - temperature);

  Serial.print("Temperature difference (C): ");
  Serial.println(temperatureDifference);

  // connect to MQTT server
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  // only publish new reading if outside repeatability range
  if (temperatureDifference >= temperatureChangeThreshold) {
    Serial.println("Temperature change detected - publishing");
    client.publish(temperature_topic, String(temperature).c_str(), true);
    previousTemperature = temperature;
  } else {
    Serial.println("No significant temperature change detected - skipping");
  }

  // drop into next reporting cycle
  Serial.println("Waiting for next reporting cycle...");
  Serial.println();
  delay(polling_ms);

} // end loop