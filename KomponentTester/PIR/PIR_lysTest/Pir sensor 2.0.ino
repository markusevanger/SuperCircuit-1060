/*
 * PIR sensor tester ***************************FUNKER IKKE? ELLER KANSKJE KODEN FUNGERER MEN IKKE PIR
 */
 
int ledPin = 13;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int pinLyd = 6;
 
void setup() {
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(pinLyd, OUTPUT);
 
  Serial.begin(9600);
}
 
void loop(){
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    tone(pinLyd, 261, 250);
    
    noTone(pinLyd);
    delay(2000);
    // we have just turned on
    Serial.println("Motion detected!");
    // We only want to print on the output change, not state

  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    noTone(pinLyd);
  
    // we have just turned of
    Serial.println("Motion ended!");
    // We only want to print on the output change, not state
    pirState = LOW;
  
  }
}