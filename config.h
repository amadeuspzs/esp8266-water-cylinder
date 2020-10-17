// Device name
#define device "device" // will prefix later parameters

// WiFi credentials.
#define wifi_hostname device "-sensor"
#define wifi_ssid "SSID"
#define wifi_password "PASSPHRASE"

// Sensor configuration
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 12
DeviceAddress tube = { 0x28, 0xAB, 0x86, 0x52, 0x0C, 0x00, 0x00, 0x89 };
DeviceAddress water = { 0x28, 0xCE, 0x91, 0x52, 0x0C, 0x00, 0x00, 0x8C };
DeviceAddress inlet = { 0x28, 0x94, 0xEE, 0x51, 0x0C, 0x00, 0x00, 0xE7 };
DeviceAddress outlet = { 0x28, 0x16, 0x2C, 0x51, 0x0C, 0x00, 0x00, 0xC2 };
DeviceAddress third = { 0x28, 0x5A, 0x7F, 0x52, 0x0C, 0x00, 0x00, 0x99 };

// Reporting
#define polling_m 1 // every X minutes
#define polling_s polling_m * 60
#define polling_ms polling_s * 1000
#define mqtt_server "0.0.0.0"
#define tube_topic device "/tube/temperature"
#define inlet_topic device "/inlet/temperature"
#define outlet_topic device "/outlet/temperature"
#define water_topic device "/water/temperature"
#define third_topic device "/third/temperature"
