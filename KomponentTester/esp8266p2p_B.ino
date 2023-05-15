#include <ESP8266WiFi.h>
#include <espnow.h>

// MAC Addresse til receiver 
uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0xE5, 0x51, 0x9F};
//E0:98:06:05:ED:C6 = MED FLEKK
//10:52:1C:E5:51:9F = UTEN FLEKk


// Definer en data struktur
typedef struct structMessage {
  bool startSignal = false;  // signal som sendes for å starte klokke.
};
// Lag et strukturert objekt
structMessage myData;


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
} 
 
void setup() {
  // Init Serial Monitor
  Serial.begin(9600); // 115200 originalt.

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  //WiFi.disconnect(); comment out???

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  // esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  bool newBoolean = true;
  myData.startSignal = newBoolean;
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  delay(1000);
  /*
  if (digitalRead(D4) == HIGH) {
    myData.startSignal = true;

    // Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    // Print incoming readings
    //printIncomingReadings();
    Serial.println("=== SENT? ===");
    delay(1000);  
  }*/
}
