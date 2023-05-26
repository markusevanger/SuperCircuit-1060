
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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, OLED_DC, OLED_RESET, OLED_CS); // gir oss et display objekt med riktige rammer

void setup() {
  Serial.begin(9600);

  setupSkjerm(); // setup kommandoer konsolidert...


  display.setTextColor(WHITE); // NØDVENDIG: vi må definere textfargen (selv om den kun har en farge, (og er blå.......))
  display.setTextSize(3); // ikke nødvendig. Standard er minste størrelse 0.
  display.setCursor(0, 10); //ikke nødvendig. Velger hvor vi starter å skrive i kordinat systemet, standard er 0, 0. 
  display.println("Super"); // print "Super" + \n (linjeskift, defor Super og Circuit er på forskeligge linjer)
  display.println("Circuit");

  
  display.display(); // oppdater innholdet på skjermen. 

}

void loop(){}



void setupSkjerm(){// kjedelig setup ting konsolidert så vi slipper å se det... 


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed")); 
    for(;;); // Don't proceed, loop forever
  }
  // ^^^^^^^^^^^^^^ idk ka dette gjør........ -markus

  display.clearDisplay(); // fjenrer det som var på skjermen før.

}