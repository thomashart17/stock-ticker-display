#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include <stdint.h>

#include "secrets.h"
#include "tickers.h"

int status = WL_IDLE_STATUS;
const char * const SERVER_URL = "query1.finance.yahoo.com";
const char * const REQUEST_TEMPLATE = "GET /v8/finance/chart/%s HTTP/1.1";
const char * const PRICE_LABEL = "regularMarketPrice";
const char * const CLOSE_LABEL = "previousClose";

WiFiSSLClient client;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(SSID, PASSWORD);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
}

void loop() {
  for (uint8_t i = 0; i < TICKER_COUNT; ++i) {
    if (client.connectSSL(SERVER_URL, 443)) {
      Serial.println("connected to server");
    }

    char request[64];
    sprintf(request, REQUEST_TEMPLATE, getTicker(i));

    Serial.println(request);
    client.println(request);
    client.println("Host: query1.finance.yahoo.com");
    client.println("Connection: close");
    client.println();

    // Wait for response
    while (!client.available());

    char buffer[2048];
    int index = 0;
    while (client.available() && index < 2048) {
      int len = client.read(buffer + index, 64); // Can only read 64 bytes at a time
      index += 64;
    }

    char * price_str = strstr(buffer, PRICE_LABEL) + strlen(PRICE_LABEL) + 2;
    double price = atof(price_str);

    char * close_str = strstr(buffer, CLOSE_LABEL) + strlen(CLOSE_LABEL) + 2;
    double close = atof(close_str);

    double increase = (price / close - 1) * 100;

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(getTicker(i));
    lcd.print(" $");
    lcd.print(price);

    lcd.setCursor(0, 1);
    if (increase > 0)
      lcd.print("+");
    lcd.print(increase);
    lcd.print("%");

    delay(5000);
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
