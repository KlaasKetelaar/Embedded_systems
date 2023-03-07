#include <WiFi.h>
#include <Wire.h>
IPAddress api_ip(145,24,238,14); // TODO: Enter here your local computer IP-address
uint16_t api_port = 8080; //  TODO: Enter here your server port
const char *device_name = "WeatherStation"; // TODO: Enter here your unique device name (don't use spaces)

// Send variables
#define SEND_PERIOD (2 * 1000)
uint32_t send_time = 0;

//#define SCL 9
//#define SDA 10
#define address 0x4A

#define LDR 24

// The wifi client instance
WiFiClient client;

char *ssid = "Tesla IoT";
char *password = "fsL6HgjN";

void connectToWiFi() {
  //Connect to WiFi Network
     Serial.println();
     Serial.println();
     Serial.print("Connecting to WiFi");
     Serial.println("...");
     WiFi.begin(ssid, password);
     int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 15)) {
     retries++;
     delay(500);
     Serial.print(".");
  }
  if (retries > 14) {
      Serial.println(F("WiFi connection FAILED"));
  }
  if (WiFi.status() == WL_CONNECTED) {
      Serial.println(F("WiFi connected!"));
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
  }
      Serial.println(F("Setup ready"));
  }

void setup() {
    // TODO: Init serial
    Serial.begin(9600);
	// TODO: Init i2c
  Wire.begin();
	// TODO: Init LDR
 pinMode(LDR, INPUT);
	// TODO: Connect to WiFi
  connectToWiFi();
}

void loop() {
    // Send measurement every send period
    if (millis() - send_time > SEND_PERIOD) {
        send_time = millis();

        // TODO: Read i2c temperature sensor into temperature
        int lightness = analogRead(LDR);
        int temperature;
        Wire.beginTransmission(address);
        Wire.write(0x00);
        Wire.requestFrom(address, 1);
        if (Wire.available()){
          temperature = Wire.read();
        }
        // TODO: Read LDR sensor and convert to percent into lightness
        

        // Send simple raw HTTP 1.1 GET request to server
        client.stop();
        if (client.connect(api_ip, api_port)) {
            // Start of any HTTP GET request
            client.print("GET /api/measurements/create?name=");
            client.print(device_name);
            client.print("&temperature=");
            client.print(temperature);
            client.print("&lightness=");
            client.print(lightness);
            client.println(" HTTP/1.1");

            // Host HTTP header: mandatory HTTP header for HTTP 1.1 request
            client.print("Host: ");
            client.print(api_ip);
            client.print(":");
            client.println(api_port);

            // Connection HTTP header: close TCP connection when HTTP request is complete
            // And an empty line so HTTP request is done
            client.println("Connection: close");
            client.println();
        } else {
            Serial.println("[HTTP] Can't send a HTTP request to the server!");
        }
    }
}
