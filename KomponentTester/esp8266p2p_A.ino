#include <ESP8266WiFi.h>
#include <espnow.h>

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
  Serial.println(myData.signal);
} 
 
void setup() {
  Serial.begin(9600); // 115200 originalt.

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
 
void loop() {

}
