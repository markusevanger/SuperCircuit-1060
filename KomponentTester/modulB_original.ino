#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI   13
#define OLED_CLK   14
#define OLED_DC    5
#define OLED_CS    15
#define OLED_RESET 4

    // OLED CODE SETUP:
// gir oss et Display objekt med riktige rammer, som heter display.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS); 
unsigned long minTeller;
unsigned long sekTeller;
unsigned long milTeller;
unsigned long elapsedTime;
String minString;
String sekString;
String milString;
String tellerString;
String forrigeString = "";


    // PIR CODE SETUP:
int inputPin = D4; // choose the input pin (for PIR sensor)
int pirState = LOW; // we start, assuming no motion detected
int val = 0; // variable for reading the pin status


    // P2P COMMUNICATION SETUP:
// MAC Addresse til receiver 
  uint8_t broadcastAddress[] = {0xE0, 0x98, 0x06, 0x05, 0xED, 0xC6};
//E0:98:06:05:ED:C6 = MED FLEKK
//10:52:1C:E5:51:9F = UTEN FLEKk

typedef struct structMessage {
  int signal; 
};

structMessage myData;
bool startSignal = false;

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.print(len);
  Serial.print(myData.signal);
  Serial.print(" === START TIMER  ");
  forrigeString = minString + ":" + sekString + ":" + milString;
  elapsedTime = 0;
  startSignal = true;
} 


void oledSetup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed")); 
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay(); // fjenrer det som var på skjermen før.
  display.setTextColor(WHITE); // vi må definere textfargen
  display.setTextSize(3);

  display.print("MOTION ");
  display.print("FINISH");
  display.display(); // oppdater skjermen med det vi definerete over. 
}


void pirSetup() {
  pinMode(inputPin, INPUT);
}

void pirLoop() {
  val = digitalRead(inputPin);
  if (val == HIGH) {
    startSignal = false;
    Serial.println("Motion detected!");
    tone(D6, 440);
    delay(1000);
    noTone(D6);
  }
  else {
    pirState = LOW;
  }
}


void startLoop() {
  display.clearDisplay(); // fjern alt innhold fra forrige loop. Skjermen er nå svart
  display.setCursor(0, 0); // Velger hvor vi starter å skrive i kordinat systemet,
  // standard er 0, 0. Om vi ikke definerer dette hver loop, vil teksten
  // "forstette å skrive" fra der forrige tekst avsluttet. 
  // i arduino kan vi ikke konkatinere, dermed skriver vi uten linjeskift. 

  oppdaterTeller();
  tellerString = minString + ":" + sekString + ":" + milString;

  display.setTextSize(3);
  display.print(tellerString);
  display.setCursor(20,40);
  display.setTextSize(2);
  display.println(forrigeString);
  display.display(); // oppdater skjermen med det vi definerete over. 
  delay(100); // ventS 1 millisekund.
}

void oppdaterTeller() {
  elapsedTime += 100;  // øk tid med 100 millisekunder
  unsigned long minTeller = (elapsedTime / 60000) % 60;
  unsigned long sekTeller = (elapsedTime / 1000) % 60;
  unsigned long milTeller = elapsedTime % 1000;

  minString = String(minTeller);
  sekString = (sekTeller < 10) ? "0" + String(sekTeller) : String(sekTeller);
  milString = "0" + String(milTeller / 100);
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
  oledSetup();
  pinMode(D6, OUTPUT);  //PIEZO
}
 
void loop() {
  pirLoop();
  if (startSignal) {
    startLoop();
  }
}
