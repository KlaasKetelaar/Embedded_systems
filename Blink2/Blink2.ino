#include <WiFi.h>

IPAddress websocket_ip(145,24,238,14); // TODO: Enter here your local computer IP-address
uint16_t websocket_port = 8080; // TODO: Enter here your websocket server port

// TODO: Create LED_UPDATE constant
const int LED_PIN= 9;
const int LED_UPDATE = 1;
// TODO: Create global led state variable
int ledState = 0;
// TODO: Create update function to set global led state and reflect the value to the microcontroller leds
void updateLedState(bool state) {
  Serial.println("update led state");
  if (state) {
    digitalWrite(LED_PIN, HIGH);
    ledState = 1;
  }
  else {
    digitalWrite(LED_PIN, LOW);
    ledState = 0;
  }
  
}

void websocket_handle_message(uint8_t *message) {
    // TODO: process incoming message and extract the message type
    Serial.println("handle message");
    Serial.println(message[1]);
    if (message[0] == LED_UPDATE){
      
      updateLedState(message[1]);
    }
    // TODO: when LED_UPDATE message type is received call update led state function with new value
}

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
    Serial.println("Starting Blink");
    // TODO: Init led pins
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    // TODO: Connect to wifi
    connectToWiFi();
    
}

void loop() {
    // A super simple websocket client (dont use in production it is not conform spec)
    // See https://datatracker.ietf.org/doc/html/rfc6455 for more info about the Websocket spec
    WiFiClient client;
    if (client.connect(websocket_ip, websocket_port)) {
        // Send HTTP request with upgrade to websocket headers
        client.println("GET / HTTP/1.1");
        client.print("Host: ");
        client.print(websocket_ip);
        client.print(":");
        client.println(websocket_port);
        client.println("Connection: Upgrade");
        client.println("Sec-WebSocket-Version: 13");
        client.println("Sec-WebSocket-Key: 2cN9hgbncbm4s4W9z0/fKQ=="); // Hard code security key always nice :)
        client.println("Upgrade: websocket");
        client.println();

        // Read intial HTTP upgrade response
        while (!(client.read() == '\r' && client.read() == '\n' && client.read() == '\r' && client.read() == '\n'));
        Serial.println("We have connected to the websocket server!");

        // Read incoming websocket frames
        while (client.connected()) {
            if (client.available() > 0) {
                // We only accept small websocket frames
                uint8_t buffer[4];
                buffer[0] = client.read(); // 0 - 3: Opcode
                buffer[1] = client.read(); // 0 - 7: Payload length
                for (int32_t i = 0; i < buffer[1] & 127; i++) {
                    uint8_t byte = client.read();
                    if (i < 2) buffer[2 + i] = byte;
                }

                // We it is a binary data frame we send it to the handle message function
                if ((buffer[0] & 0xf) == 2) {
                    websocket_handle_message(&buffer[2]);
                }
            }
        }

        // When the client disconnects we stop
        client.stop();
        Serial.println("We have disconnect from the websocket server!");
    } else {
        Serial.println("Can't connect to the websocket server!");
    }

    // After some delay we reconnect with the Websocket server
    delay(2500);
}
