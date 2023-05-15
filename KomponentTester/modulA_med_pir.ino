#include <ESP8266WiFi.h>
#include <espnow.h>

    // PIR CODE SETUP:
int ledPin = 13; // choose the pin for the LED
int inputPin = 2; // choose the input pin (for PIR sensor)
int pirState = true; // we start, assuming no motion detected
int val = 0; // variable for reading the pin status
int minimummSecsLowForInactive = 5000; // If the sensor reports low for
// more than this time, then assume no activity
long unsigned int timeLow;
boolean takeLowTime;
 
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;


    // P2P COMMUNICATION SETUP:
// MAC Addresse til receiver 
uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0xE5, 0x51, 0x9F};
//E0:98:06:05:ED:C6 = MED FLEKK
//10:52:1C:E5:51:9F = UTEN FLEKk

typedef struct structMessage {
  int signal; 
};

structMessage myData;

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print(" " + myData.signal + " === START TIMER");
} 

void pirSetup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inputPin, INPUT);
  
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

void pirLoop() {
  void pirLoop() {
  val = digitalRead(inputPin);
  if (val == HIGH) {
    digitalWrite(ledPin, HIGH);


    // Her skal timeren stoppe og resultat vises på skjerm.
    Serial.println("Timer stopped");


    if (pirState) {
      pirState = false;
      Serial.println("Motion detected!");
      delay(50);
    }
    takeLowTime = true;
  }
  else {
    digitalWrite(ledPin, LOW);
    if (takeLowTime) {
      timeLow = millis();
      takeLowTime = false;
    }
    if (!pirState && millis() - timeLow > minimummSecsLowForInactive) {
      pirState = true;
      Serial.println("Motion ended!");
      delay(50);
    }
  }
}

void wifiSetup() {
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void setup() {
  Serial.begin(9600);
  pirSetup();
  wifiSetup();
}
 
void loop() {

}
