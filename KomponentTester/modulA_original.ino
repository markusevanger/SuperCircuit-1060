#include <ESP8266WiFi.h>
#include <espnow.h>

    // PIR CODE SETUP:
int inputPin = D5; // vel pin for PIR
int pirState = LOW; // vi starter, og antar at motion ikke er merket
int val = 0; // variabel for å lese pin status
    // PIEZO CODE SETUP
int b = 720; // frekvens for noten B


    // P2P COMMUNICATION SETUP:
// MAC Addresse til receiver 
uint8_t broadcastAddress[] = {0x10, 0x52, 0x1C, 0xE5, 0x51, 0x9F};
//E0:98:06:05:ED:C6 = MED FLEKK
//10:52:1C:E5:51:9F = UTEN FLEKK

// Definer en data struktur
typedef struct structMessage {
  int signal = 1;  // signal som sendes for å starte klokke.
};

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

void pirSetup() {
  pinMode(inputPin, INPUT);     // declare sensor as input
}

void pirLoop() {
  val = digitalRead(inputPin);
  if (val == HIGH) {
    // MENS MOTION ER MERKET
    Serial.println("Motion detected!");
    // Sender WiFi signal:
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData)); 
    // Spiller av lyd
    tone(D6, b);
    delay(1000);
    noTone(D6);
    delay(9000);
      
  } else {  
    pirState = LOW;
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
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}
 
void setup() {
  Serial.begin(9600);
  pirSetup();
  wifiSetup();
  pinMode(D6, OUTPUT); // PIEZO
}
 
void loop() {
  pirLoop();
}
