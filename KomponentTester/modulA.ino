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
bool started = false;


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
  Serial.println(myData.signal);
  Serial.print(" === START TIMER");
} 


void oledSetup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed")); 
    for(;;); // Don't proceed, loop forever
  }
  // ^^^^^^^^^^^^^^ idk ka dette gjør........ -markus
  display.clearDisplay(); // fjenrer det som var på skjermen før.
  display.setTextColor(WHITE); // NØDVENDIG: vi må definere textfargen (selv om den kun har en farge, (og er blå.......))
  display.setTextSize(3); // Vertical size (y-axis)
}

void oledLoop() {
  display.clearDisplay(); // fjern alt innhold fra forrige loop. Skjermen er nå svart
  display.setCursor(0, 0); // Velger hvor vi starter å skrive i kordinat systemet, standard er 0, 0. Om vi ikke definerer dette hver loop, vil teksten "forstette å skrive" fra der forrige tekst avsluttet. 
  // i arduino kan vi ikke konkatinere, dermed skriver vi uten linjeskift. 
 
  oppdaterTeller();
  tellerString = minString + ":" + sekString + ":" + milString;
  display.print(tellerString); 
  display.display(); // oppdater skjermen med det vi definerete over. 
  delay(100); // vent 1 sek
}

void oppdaterTeller() {
  elapsedTime += 100;  // Increment elapsed time by 100 milliseconds
  unsigned long minTeller = (elapsedTime / 60000) % 60;
  unsigned long sekTeller = (elapsedTime / 1000) % 60;
  unsigned long milTeller = elapsedTime % 1000;
  Serial.println(sekTeller);

  minString = String(minTeller);
  sekString = (sekTeller < 10) ? "0" + String(sekTeller) : String(sekTeller);
  milString = "0" + String(milTeller / 100);
  
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
  val = digitalRead(inputPin);
  if (val == HIGH) {
    digitalWrite(ledPin, HIGH);


    // Her skal timeren stoppe og resultat vises på skjerm.
    Serial.println("Timer stopped");


    if (pirState) {
      pirState = false;
      Serial.println("Motion detected!");
      delay(10000);
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
  //pirSetup();
  wifiSetup();
  oledSetup();
}
 
void loop() {
  //pirLoop();
  oledLoop();
}
