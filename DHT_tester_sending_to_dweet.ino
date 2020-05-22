//Edited by chinua
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTPIN 5     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
// WiFi parameters
const char* ssid = "wifi name"; //Access point name
const char* password = "password";  //wifi password

// Host
const char* host = "dweet.io";

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  delay(1000);
   Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
   Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  delay(5000);
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
// while (!client.connect(host, httpPort)) {
   // Serial.println("connecting");
   Serial.println("connection failed");
   delay(5000);
    return;
  }
  // This will send the request to the server
 // client.print(String("GET /dweet/for/kenchTech01?temperature=")+ (String(t)) + "&humidity=" + (String(h)) + " HTTPS/1.1\r\n" +
           //    "Host: " + host + "\r\n" + 
            //   "Connection: close\r\n\r\n");
                String dweetstring="GET /dweet/for/";
  dweetstring=dweetstring+"kenchTech01"+"?";
  dweetstring=dweetstring+"temp=";
  dweetstring=dweetstring+(t);
  dweetstring=dweetstring+"&humidity=";
  dweetstring=dweetstring+(h);
  dweetstring=dweetstring+" HTTP/1.1\r\n"+
      "Host: " + 
      host + 
      "\r\n" + 
      "Connection: close\r\n\r\n";
      client.print(dweetstring);
  delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.connected()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    delay(1000);
  }
   Serial.print("Temperature: ");
   Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
  Serial.println("closing connection");
  
  // Repeat every 10 seconds
  delay(10000);  
  
  }
