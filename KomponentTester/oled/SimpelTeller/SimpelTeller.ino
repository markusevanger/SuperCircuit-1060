
// importere nødvendige biblioteker...
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// set porter riktig i forhold til arduinobrettet
#define OLED_DC     8
#define OLED_CS     10
#define OLED_RESET  9


// gir oss et Display objekt med riktige rammer, som heter display.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS); 



int teller = 0; //tid takeren vår for telleren. 

void setup() {
  Serial.begin(9600);

  setupSkjerm(); // setup kommandoer konsolidert...

  display.setTextColor(WHITE); // NØDVENDIG: vi må definere textfargen (selv om den kun har en farge, (og er blå.......))
  display.setTextSize(2); // ikke nødvendig. Standard er minste størrelse 0.


}


void loop(){


  display.clearDisplay(); // fjern alt innhold fra forrige loop. Skjermen er nå svart

  display.setCursor(0, 0); // Velger hvor vi starter å skrive i kordinat systemet, standard er 0, 0. Om vi ikke definerer dette hver loop, vil teksten "forstette å skrive" fra der forrige tekst avsluttet. 
  
  // i arduino kan vi ikke konkatinere, dermed skriver vi uten linjeskift. 
  display.print("Teller:");
  display.print(teller); // skriver ut verdien av teller 

  display.display(); // oppdater skjermen med det vi definerete over. 

  delay(1000); // vent 1 sek
  teller++; // oek teller med 1. 

}



void setupSkjerm(){// kjedelig setup ting konsolidert så vi slipper å se det... 


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed")); 
    for(;;); // Don't proceed, loop forever
  }
  // ^^^^^^^^^^^^^^ idk ka dette gjør........ -markus

  display.clearDisplay(); // fjenrer det som var på skjermen før.

}