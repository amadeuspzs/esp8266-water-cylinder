/*
 * esp8266 with DS18B20+ (mains-powered)
 *
 * MQTT Temperature Sensor
 *
 * Designed for integration with HomeAssistant
 *
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.h" // WiFi and device settings

WiFiClient espClient;
PubSubClient client(espClient);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

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

  // Start up the Dallas library
  sensors.begin();

} // end setup

void loop() {

  // read latest sensors
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  sensors.setResolution(TEMPERATURE_PRECISION);
  
  float tubeC = sensors.getTempC(tube);
  float waterC = sensors.getTempC(water);
  float inletC = sensors.getTempC(inlet);
  float outletC = sensors.getTempC(outlet);
  float thirdC = sensors.getTempC(third);
  
  Serial.print("Tube temperature: ");
  Serial.print(tubeC);
  Serial.println("C");

  Serial.print("Water temperature: ");
  Serial.print(waterC);
  Serial.println("C");
  
  Serial.print("Inlet temperature: ");
  Serial.print(inletC);
  Serial.println("C");

  Serial.print("Outlet temperature: ");
  Serial.print(outletC);
  Serial.println("C");

  Serial.print("Third temperature: ");
  Serial.print(thirdC);
  Serial.println("C");

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

  Serial.println("Publishing to MQTT...");
  
  // publish new reading
  if (tubeC > 0) client.publish(tube_topic, String(tubeC).c_str(), true);
  if (waterC > 0) client.publish(water_topic, String(waterC).c_str(), true);
  if (inletC > 0) client.publish(inlet_topic, String(inletC).c_str(), true);
  if (outletC > 0) client.publish(outlet_topic, String(outletC).c_str(), true);
  if (thirdC > 0) client.publish(third_topic, String(thirdC).c_str(), true);
  
  // drop into next reporting cycle
  Serial.println("Waiting for next reporting cycle...");
  Serial.println();
  delay(polling_ms);

} // end loop
