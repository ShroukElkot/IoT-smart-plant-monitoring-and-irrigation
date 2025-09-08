#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/RTDBHelper.h"
#include "addons/TokenHelper.h"
#include <SoftwareSerial.h>

// Define pin numbers
const int moistureSensorPin = A0;  // Moisture sensor connected to A0
const int pumpPin = D1;            // Pump connected to digital pin D1

// Define threshold
const int moistureThreshold = 500; // Default threshold value

// Firebase credentials
#define API_KEY "AIzaSyCmBB0qzFZoQj-N77UHjn_AUra2HXnbmak"
#define DATABASE_URL "https://lab6-a9984-default-rtdb.firebaseio.com/"

// Wi-Fi credentials
#define ssid "iPhone"
#define pass "shrouk00"

// RX, TX = D5 (GPIO14), D6 (GPIO12)
SoftwareSerial mySerial(14, 12);  // mySerial RX, TX

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupok = false;
unsigned long sendDataPrevMillis = 0;

const int ledPin = LED_BUILTIN;  // Built-in LED pin (usually GPIO2 on ESP8266)

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(pumpPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  Serial.println("ESP8266 Initialized and Waiting for Data...");
  Serial.println("ESP Ready to Receive from Tiva C...");

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println(WiFi.localIP());

  // Firebase setup
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase SignUp OK");
    signupok = true;
  } else {
    Serial.printf("Firebase SignUp Failed: %s\n", fbdo.errorReason().c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  int moistureValue = analogRead(moistureSensorPin);

  // Send moisture data to Firebase
  if (signupok) {
    if (!Firebase.RTDB.setInt(&fbdo, "moisture/value", moistureValue)) {
      Serial.print("Moisture set failed: ");
      Serial.println(fbdo.errorReason());
    }

    // Get threshold from Firebase
    int currentThreshold = moistureThreshold;
    if (Firebase.RTDB.getInt(&fbdo, "settings/threshold")) {
      if (fbdo.dataType() == "int") {
        currentThreshold = fbdo.intData();
      }
    } else {
      Serial.print("Threshold read failed: ");
      Serial.println(fbdo.errorReason());
    }

    // Control pump
    bool pumpStatus = (moistureValue > currentThreshold);
    digitalWrite(pumpPin, pumpStatus ? HIGH : LOW);

    // Update pump status in Firebase
    if (!Firebase.RTDB.setBool(&fbdo, "pump/status", pumpStatus)) {
      Serial.print("Pump status set failed: ");
      Serial.println(fbdo.errorReason());
    }

    // Debug output
    Serial.print("Moisture: ");
    Serial.print(moistureValue);
    Serial.print(" | Threshold: ");
    Serial.print(currentThreshold);
    Serial.print(" | Pump: ");
    Serial.println(pumpStatus ? "ON" : "OFF");
  }

  delay(1000);
}
